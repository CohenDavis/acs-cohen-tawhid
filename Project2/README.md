Course Mini-Project #2: Programming with Multiple Threads

# Overview

This program is designed to run in a Linux environment, using pthreads for multithreading functionality. An Ubuntu bash terminal in Windows can also be used.

Install zlib library using these instructions: https://www.systutorials.com/how-to-install-the-zlib-library-in-ubuntu/#note-about-linking-cc-programs-with-zlib

If not already installed, install the pthread library with:
```
sudo apt-get install libpthread-stubs0-dev
```
Compress.cpp will use multithreaded programming to compress a file using the number of worker threads specified by user input. Run compress.cpp with this command:

```
g++ -g compress.cpp -lz -std=c++11 -pthread -o compress.out && ./compress.out
```
The code will then ask the user for two inputs. It will ask the user for the name of a file they would like to compress and then the number of worker threads they would like to use.

After the inputs are taken in, the code will compress the input file using the zlib library, wait for threads to rejoin, then output the compressed file. If any errors writing to the output file are encountered during repeated, back-to-back testing, it is recommended to delete the output file and run the program again.

Zlib_compression.cpp is a file used for testing/experimentation, and will compress then decompress a text file and output the results. While compress.cpp contains all necessary functionality for this project, curious parties can run zlib_compression.cpp with this command where ex.txt is input text file:

```
g++ -g zlib_compression.cpp -lz -o x && ./x "ex.txt"
```

# Data Compression

Data compression will be done using the zlib library. First the code will convert the data in the file into a vector of unsigned characters in order to run the multiple thread feature. The code will then convert this vector into a string and process it using zlib library. The compression function will use the default Z_BEST_COMPRESSION as compression level and compress the input string. After deflating the string, it will reconvert this output string into a vector of unsigned characters again so the worker threads can finish their process.

The following code demonstrates how the program will convert between unsigned character vectors and strings during compression process:

```
std::vector<unsigned char> uncompressed = thread_data->uncompressed_vec;
std::vector<unsigned char> compressed;
std::string temp_uncompressed(uncompressed.begin(), uncompressed.end());
std::string temp_compressed = compress_string(temp_uncompressed);
for (int i= 0 ;i< temp_compressed.size(); ++i) {
         compressed.push_back(temp_compressed[i]);
}
thread_data->compressed_vec = compressed;
```

The following code is the core algorithm for file compression:

```
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
```

It will first take the input string str and set it as input for z_stream object zs. It will then deflate the object byte by byte and record the output into outbuffer. This function can only compress blocks of data one at a time so that the compressed block is a maximum of 80k bytes. The decompression process is nearly identical as the compression process except the z_stream object will be inflated instead of being deflated.

# Multithreading

After input file data is read in, the program enters a loop which controls the creation of threads as the input data is parsed and processed. To simplify the passing of data to and from threads, a nested loop creates the user-specified number of threads, waits for them to execute, joins them back to the main thread, and then writes the data processed from those threads to the output. After this inner loop completes, the outer loop restarts the inner loop, creating a new set of threads (the user-specified number) in place of the old threads. Variables outside of this outer loop are used to keep track of how much of the input data has been read, as 4KB chunks of this input are passed to the threads when created. These threads then call the zlib_compression function (which in turn calls a helper function) to facilitate data compression. For simplicity, thread data is stored in a thread_input_output struct, which holds both the uncompressed and compressed data for each thread; this greatly simplified the passing of arguments and returning of results for each thread. Once the entire input file has been read (and the compressed data written to output), the outer loop exits, and a message (including time taken to compress) is output to signal program completion.

# Performance Testing

### Hardware Used During Testing

Dell XPS-9560:
* i7-7700HQ (4C/8T, 2.80GHz Base/3.80GHz Boost)
* GTX 1050 (2GB GDDR5)
* 16GB RAM
* 512 GB NVMe SSD
* Windows 10 64-bit

### Testing Results

In testing the performance of the multithreaded compression algorithm, an approximately 6.8GB "mp4" file was utilized as the input. A larger 13.8 GB "mp4" file was confirmed to work, though the smaller 6.8GB file was opted for during full testing in the interest of time. Upon completing an initial single-threaded test with the 6.8GB file, thread counts from 2 to 8 were also tested. Considering the trend seen while testing lower thread counts, along with the fact that the CPU of the computer used only possesses 8 logical cores, testing above 8 threads was deemed unnecessary for the purposes of this assignment. Times taken to compress the input and output the compressed data are tabulated below. Percent speed-up relative to a single thread is also provided, and was calculated using % SPEED-UP = |(NEW - OLD)|/OLD x 100. Speed-ups (less time taken) are marked as positive, while slow-downs (longer time taken) are marked as negative speed-ups.

Thread Count | Time to Compress (seconds) | Percent speed-up (%)*
------------ | ------------- | -------------
1 | 548.0 | 0 (N/A)
2 | 498.3 | 9.1
3 | 505.4 | 7.8
4 | 573.6 | -4.7
5 | 628.0 | -14.6
6 | 659.8 | -20.4
7 | 677.5 | -23.6
8 | 717.8 | -31.0

*Note - A positive percent speed-up indicates a decrease in compression time relative to a single thread, while a negative value indicates an increase

# Analysis & Conclusions

As shown in the table above, the compression algorithm's best performance was seen when configured to run with 2 threads. Here, the program saw just under a 10% performance improvement (498 seconds compared to 548 seconds) versus the single-threaded test. Running with 3 threads also showed decent improvement, at just under 8%. When configured with 4 threads, however, the algorithm ran slightly slower than the single-threaded version, at about 574 seconds. This trend continued, showing a more and more severe performance impact/decrease as additional threads were added, with the 8-thread configuration showing a 31% performance decrease relative to the 1-thread test. While it was anticipated that there would be a performance "sweet spot", and that performance would not continually improve when configured with a high number of threads, this severe performance penalty for high thread usage was somewhat unexpected. One interesting observation worthy of note, was that when testing with higher thread counts (6-8), the laptop used would generate significantly more heat and system fans would spin up accordingly. Meanwhile, though the system's CPU ran at approximately 3.7 GHz during the low thread count tests (1-3), the CPU throttled down to approximately 3.3 GHz once thermally constrained during the high thread count tests. Theoretically, this could decrease performance for individual threads; however, given the (relatively) minor drop in frequency, this performance decrease was likely minimal, only exacerbating already present issues/phenomena. In regards to other potential causes, it is possible that the compression method used in the compress_string helper function could use further modification or optimization, particularly given the vector<unsigned char> data storage method used through the majority of the rest of the code (the conversion to string may be a slow-down point here). Aside from that, it is also possible that the thread creation and joining scheme could use modification. During the thread joining process, "faster" threads do have to wait for "slower" threads to complete their process before compressed data output occurs. To use a metaphor, this could act as a "stop sign" and cause "traffic jams" during high thread count runs. The process of taking in uncompressed data from the input file could also surely be optimized, allowing for the more rapid delivering of data to threads, rather than waiting for a full file read-in before allocating data chunks.

Despite performance degradation with higher thread counts, the multithreaded algorithm still managed to show improvement when run with multiple threads (optimally 2) versus its standard single-thread performance. It should be noted that the "mp4" file used was successfully compressed from 6,794MB to 6,783MB. While this may not seem like much, compression quantity is certainly going to depend on file size and format. Along with the potential code modifications already touched on, further testing with larger files and various file types could be completed were this project to be developed further. Overall, however, this program was successful at showcasing multithreading techniques, in the context of data compression, and how these techniques can impact program performance.
