Course Mini-Project #2: Programming with Multiple Threads

# Overview

Install zlib library using this instructions: https://www.systutorials.com/how-to-install-the-zlib-library-in-ubuntu/#note-about-linking-cc-programs-with-zlib

zlib_compression.cpp will compress then decompress a text file and output the results. Run zlib_compression.cpp with this command where ex.txt is input text file:

```
g++ -g zlib_compression.cpp -lz -o x && ./x "ex.txt"
```

Compression.cpp will use multithreaded programming to compress a file using number of worker threads specified by user input. Run compression.cpp with this command:

```
g++ -g compression.cpp -lz -o compression.out && ./compression.out
```
The code will then ask the user for two inputs. It will ask the user for name of a file they would like to compress and then the number of worker threads they would like to use. 

After the inputs are taken in, the code will compress the input file using the zlib library, wait for threads to rejoin, then output the compressed file. 

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

It will first take the input string str and set it as input for z_stream object zs. It will then deflate the object byte by byte and record the output into outbuffer. This function can only compress blocks of data one at a time so that the compressed block is maximum of 80k bytes. The decompression process is nearly identical as the compression process except the z_stream object will be inflated instead of being deflated.
