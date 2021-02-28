#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include "zlib.h"
#include <string.h>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <iterator>
#include "zlib.h"
#include "pthread.h"

#define NUM_BYTES 4096
struct thread_input_output {
    std::vector<unsigned char> uncompressed_vec; //uncompressed data for thread
    std::vector<unsigned char> compressed_vec; //compressed output from thread
};

void *zlib_compression(void *param) {

  struct thread_input_output *thread_data;
  thread_data = (struct thread_input_output *)param;
  std::vector<unsigned char> uncompressed = thread_data->uncompressed_vec;
  std::vector<unsigned char> compressed;



  //INSERT COMPRESSION ALGO, PUT IN "compressed" VECTOR//



  thread_data->compressed_vec = compressed;
  pthread_exit(NULL);
}


int main(int argc, char** argv){
  std::string input_file;
  int num_threads;

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
  std::ofstream outstream("test_output", std::ofstream::binary);

  //reads bytes from file into unsigned char vector
	std::vector<unsigned char> input_data((std::istreambuf_iterator<char>(instream)), (std::istreambuf_iterator<char>()));
	//std::cout << input_data.size() << " bytes read" << std::endl;

  long long int begin_read = 0;
  long long int end_read = NUM_BYTES;
  bool done_read = false;
  int error_code = 0;
  void *status;

  clock_t Time1 = clock();
  while (!done_read) {

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
      std::vector<unsigned char>::iterator it_b = input_data.begin() + begin_read;
      std::vector<unsigned char>::iterator it_e = input_data.begin() + end_read;
      std::vector<unsigned char> buffer (it_b, it_e); //sets up 4KB (usually) buffer
      thread_data[t].uncompressed_vec = buffer;

      //Create threads
      error_code = pthread_create(&threads[t], &attr, zlib_compression, (void *)&thread_data[t]);
      if (error_code){
        printf("ERROR; return code from pthread_create() is %d\n", error_code);
      }

      active_threads++;
      begin_read += NUM_BYTES;
      end_read += NUM_BYTES;

    }

    //wait for threads to finish and rejoin
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

//SCRAP CODE, WILL BE DELETED LATER
/*
read data
    loop
      create threads
      do work, join
      write
      close threads
      break if all data read
      loop again to create more threads

*/


/*  std::vector<char> test (4096, 'c');
  //set attribute allowing threads to be joinable
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  int rc = 0;
  long t = 0;
  for(t=0; t<num_threads; t++){
    rc = pthread_create(&threads[t], &attr, zlib_compression, &test);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
    }
  }
    void *status;
  for(t=0; t<num_threads; t++){
    pthread_join(threads[t], &status);
  } */



  /*// Stop eating new lines in binary mode!!!
  instream.unsetf(std::ios::skipws);

  // get its size:
  std::streampos file_size;

  instream.seekg(0, std::ios::end);
  file_size = instream.tellg();
  instream.seekg(0, std::ios::beg);

  // reserve capacity
  std::vector<unsigned char> input_data;
  input_data.reserve(file_size);

  // read the data:
  input_data.insert(input_data.begin(),
             std::istream_iterator<unsigned char>(instream),
             std::istream_iterator<unsigned char>());*/
