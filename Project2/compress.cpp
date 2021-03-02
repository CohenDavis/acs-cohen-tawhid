#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string.h>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <iterator>
#include "zlib.h"
#include "pthread.h"

#define NUM_BYTES 4096
struct thread_input_output { //thread data storage mechanism
    std::vector<unsigned char> uncompressed_vec; //uncompressed data for thread
    std::vector<unsigned char> compressed_vec; //compressed output from thread
};

//compression helper function called by zlib_compression function
std::string compress_string(const std::string& str,
                            int compressionlevel = Z_BEST_COMPRESSION)
{
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (deflateInit(&zs, compressionlevel) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();           // set the z_stream's input
    int ret;
    char outbuffer[80000]; // buffer size is set to 80k bytes
    std::string outstring;
    // retrieve the compressed bytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // append the block to the output string
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

//decompression function used for debugging - never called in code
std::string decompress_string(const std::string& str)
{
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[80000];
    std::string outstring;


    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") "
            << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

//function called by threads to perform compression
void *zlib_compression(void *param) {

  struct thread_input_output *thread_data;
  thread_data = (struct thread_input_output *)param;
  std::vector<unsigned char> uncompressed = thread_data->uncompressed_vec;
  std::vector<unsigned char> compressed;
  std::string temp_uncompressed(uncompressed.begin(), uncompressed.end());
  std::string temp_compressed = compress_string(temp_uncompressed);
  for (int i= 0 ;i< temp_compressed.size(); ++i) {
           compressed.push_back(temp_compressed[i]);
  }
  thread_data->compressed_vec = compressed;
  pthread_exit(NULL);
}

//main function serves as task delegation thread
int main(int argc, char** argv){
  std::string input_file;
  int num_threads;

  //get user inputs/preferences
  std::cout << "Input name of file to be compressed (ensure in same directory as this compression program): ";
  std::cin >> input_file;
  std::cout << "Input number of worker threads: ";
  std::cin >> num_threads;

  pthread_t threads[num_threads];
  struct thread_input_output thread_data[num_threads];
  //set attribute allowing threads to be joinable
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  //set up in/out file streams
  std::ifstream instream(input_file, std::ios::binary);
  std::ofstream outstream("output_file", std::ofstream::binary);

  //reads bytes from file into unsigned char vector
	std::vector<unsigned char> input_data((std::istreambuf_iterator<char>(instream)), (std::istreambuf_iterator<char>()));

  long long int begin_read = 0;
  long long int end_read = NUM_BYTES;
  bool done_read = false;
  int error_code = 0;
  void *status;

  clock_t Time1 = clock();
  while (!done_read) { //continue looping through, creating num_threads threads at a time while uncompressed data remains

    int active_threads = 0;
    for(int t=0; t<num_threads; t++){

      //check loop escape conditions
      if (end_read >= input_data.size()) {
        //if nothing to read, break
        if (begin_read >= input_data.size()) {
          break;
        }
        //if one smaller chunk left, set end_read to allow for smaller buffer
        else {
          end_read = input_data.size();
        }
        done_read = true;
      }

      //store chunk of input file data in buffer, copy buffer to thread data
      std::vector<unsigned char> buffer (input_data.begin() + begin_read, input_data.begin() + end_read);
      thread_data[t].uncompressed_vec = buffer;

      //Create threads
      error_code = pthread_create(&threads[t], &attr, zlib_compression, (void *)&thread_data[t]);
      if (error_code){
        printf("ERROR; return code from pthread_create() is %d\n", error_code);
      }

      active_threads++; //to keep track of active threads in case not enough data to occupy all desired threads
      begin_read += NUM_BYTES;
      end_read += NUM_BYTES;

    }

    //rejoin threads once they complete compression task on 4KB chunk
    for(int t=0; t<active_threads; t++) {
      error_code = pthread_join(threads[t], &status);
      if (error_code) {
        printf("ERROR; return code from pthread_join() is %d\n", error_code);
      }
    }

    //write thread data to output file
    for(int t=0; t<active_threads; t++) {
      char* output = reinterpret_cast<char*>(thread_data[t].compressed_vec.data());
      outstream.write(output, thread_data[t].compressed_vec.size());
    }

  }

  //calculate time taken to complete compression
  clock_t Time2 = clock();
  float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC;
  std::cout << "Compression complete" << std::endl;
  printf("Time taken for compression is %.7f \n", TotalTimeLoop);

  //close input/output streams
  instream.close();
  outstream.close();

  pthread_exit(NULL);
}
