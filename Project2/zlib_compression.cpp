/*//////////////////////////////////////////////////////////////////////////////////////////////////

THIS IS A TESTING FILE - DO NOT USE - THE ONLY FILE NEEDED TO EXECUTE (ASIDE FROM AN INPUT TO
BE COMPRESSED) IS COMPRESS.CPP

/////////////////////////////////////////////////////////////////////////////////////////////////*/

#include <string.h>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sstream>
#include "zlib.h"

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


int main(int argc, char* argv[])
{
    std::string input;
  	std::ifstream f(argv[1]);//taking first argument as input file name
  	if(f) {
      std::ostringstream ss;
      ss << f.rdbuf(); // reading data into input string using ostringstream buffer
      input = ss.str();
    }

    std::string cstr = compress_string(input); // compress input into cstr
    std::cout << "Compressing data from: "
              << input.size() << " bytes -> " << cstr.size()
              << " bytes (" << std::setprecision(1) << std::fixed
              << ( (1.0 - (float)cstr.size() / (float)input.size()) * 100.0)
              << "% decrease).\n";
    std::cout << "Print compressed:\n";
    std::cout << cstr;
    std::cout << "\n\n\n";
    std::cout << "Decompressing data from : "
              << cstr.size() << " bytes -> " << input.size()
              << " bytes (" << std::setprecision(1) << std::fixed
              << ( (((float)input.size() - (float)cstr.size())/ (float)cstr.size() ) * 100.0 )
              << "% increase).\n";
    cstr = decompress_string(cstr); // decompress string into original string
    std::cout << "Print decompressed:\n";
    std::cout << cstr;
    std::cout << "\n";

}
