Course Mini-Project #4: Implementation of Dictionary Encoder

# Overview

This program is designed to run in a Linux environment. An Ubuntu bash terminal in Windows can also be used. To compile and run the program, the following command may be used:
```
g++ -g dict_encode.cpp -std=c++11 -o dict_encode.exe && ./dict_encode.exe
```
The code will then ask the user for two inputs. It will ask the user if they want to encode, query, or decode a file and then ask for which file to run the operation on. In the case of the query operation, the program will also ask for an item/"word" to be queried for.

# Encoding

This feature will convert a raw column data file into a file that contains a dictionary, which specifies the conversion between raw data items and their encoded equivalents, along with the encoded column data. In this encoded file, a single semicolon is used as a delimiting character, in order to signal the end of the dictionary data and the beginning of the encoding data. If the user selects to encode a file, then a function will first read the raw data from the file into a dictionary. This dictionary utilizes the STD Map data structure which is an ordered associative data structure and allows O(logN) access time. The key-value pair of this dictionary will be the original data item (key) and a pair (value) containing the location of this data's first occurrence, as well as the frequency of that item's occurrence in the provided raw file. The next step in this process is to use the dictionary to encode the raw data. Here, a raw data item's encoded equivalent is the line number of that data item's first occurrence in the original/raw file (a unique characteristic in a column file). Thus, after dictionary construction is complete and the dictionary is written to the "encoded_file" output, the original file is read once more, and each data item is replaced with its dictionary-specified value when written to the output file.

# Querying

This feature allows the user to look up a string data value in the encoded file. This function will take in the string data they want to look up, along with an encoded file. A function will first create a dictionary from the encoded file. The encoded file contains the string data, the location of the data, and the number of times the data appears, so it is simple to convert these items from the file into a key-value pair in order to populate the dictionary. Next, the query feature will search this dictionary to determine if the string data given by the user exists in the assembled dictionary. If it exists then it will output how many times the string occurs in the file. Because this number of occurrences data is stored in the dictionary (incurring some storage cost for encoded files), returning this information to the user is very straightforward, yielding desirable performance benefits for the querying function.

# Decoding

This feature will decode an encoded file into the original file it was encoded from. It will first create/rebuild a dictionary from the encoded file, along with another supplemental map-based structure that uses the initial location of a data item (key) and the string data itself (value) for the key-value pair. This supplemental structure makes the conversion from encoded data back to raw data significantly easier. After these dictionary structures are populated, the original file will be reconstructed. Here, the original file will be created by reading through the encoded column data and placing the string data from the dictionary at its correct location also given by the dictionary. The decoded data is written to the "decoded_file" output file. This decoding functionality is not a key feature of the program, and was more so used in order to confirm the validity of the encoding process. No extensive performance tests on the decoding functionality have been performed, but users are welcome to leverage this function for their own sanity-checks.  

# Performance Testing

### Hardware Used During Testing

Dell XPS-9560:
* i7-7700HQ (4C/8T, 2.80GHz Base/3.80GHz Boost)
* GTX 1050 (2GB GDDR5)
* 16GB RAM
* 512 GB NVMe SSD
* Windows 10 64-bit

### Testing Results

In testing the performance of the dictionary encoder, both encoding and querying performance tests were completed. For the encoding tests, the provided "Small-Size-Columns" (931 KB), "Medium-Size-Columns" (9.2 MB), and "Large-Size-Columns" (179.8 MB) raw data column files were used,  with three encoding trials being run for each file (and then averaged). For the querying tests, these provided files were first encoded, yielding small, medium, and large encoded files containing the dictionary and encoded column data. Various queries were then performed on each of these encoded files. Specifically, for each file size, three random items which were known to exist in the corresponding files were queried for. Then, again for each file size, a fourth item which was known not to exist in the files was queried for (in order to get a point of comparison). The program outputs, detailing the number of occurrences of the query item, along with the time taken to generate those results, were then tabulated below.

#### Encoding Times
File Encoded | Trial 1 (seconds) | Trial 2 (seconds) | Trial 3 (seconds) | Average (seconds)
------------ | ------------- | ------------- | ------------- | -------------
Small-Size-Column.txt | 0.75 | 0.75 | 0.75 | 0.75
Medium-Size-Column.txt | 8.28 | 8.17 | 8.23 | 8.23
Large-Size-Column.txt | 192.11 | 191.67 | 191.47 | 191.75

#### Query Times
File Size | Item Queried | Output | Time (seconds)
------------ | ------------- | ------------- | -------------
Small | vomzhkkfrh | vomzhkkfrh occurs 110 times in the provided file | 0.000
Small | kkcyzdksev | kkcyzdksev occurs 96 times in the provided file | 0.000
Small | oqtcvbctp | oqtcvbctp occurs 81 times in the provided file | 0.000
Small | abcdef | The item you entered is not in the provided file | 0.000
Medium | ynryjgfjmm | ynryjgfjmm occurs 102 times in the provided file | 0.031
Medium | qcnctiffny | qcnctiffny occurs 89 times in the provided file | 0.016
Medium | dgeh | dgeh occurs 97 times in the provided file | 0.031
Medium | abcdef | The item you entered is not in the provided file | 0.031
Large | lwqjabamf | lwqjabamf occurs 80 times in the provided file | 0.578
Large | dhhnpzkqtg | dhhnpzkqtg occurs 98 times in the provided fil | 0.578
Large | eflpmunykt | eflpmunykt occurs 88 times in the provided file | 0.594
Large | abcdef | The item you entered is not in the provided file | 0.578

# Analysis & Conclusions

Looking at the above results from the encoding tests, performance of the dictionary encoder appears to scale roughly proportionally to the column file size. The "Medium-Size-Column" file, for example, is 9.88 times larger in size than the "Small-Size-Column" file. Meanwhile, the average encoding time for the medium file was 10.97 times greater than that of the small file. As for the "Large-Size-Column" file, this file is 19.54 times larger than the medium file; it's average encoding time, meanwhile, was 23.29 times longer than that of the medium file.
Looking at the query test results, all queries performed took under one second to complete, and were fairly consistent for each respective file size. For the small encoded file, the average query time was 0.000 seconds, or somewhere under one millisecond. For the medium encoded file, this average time was 0.027 seconds, and for the large encoded file, this was 0.582 seconds. As previously discussed, the query time is able to be so low due to the storing of item frequencies (number of occurrences) during the construction of the dictionary. Due to the 0.000 second results for the small file, it is difficult to get a useful measure of how query performance scales from the small to medium files. From the medium to large file, however, the increase in time taken from 0.027 seconds to 0.582 seconds is an approximately 21.56 times increase. This is similar to the encoding time increase from the raw medium to large files (23.29x) and the file size increase from medium to large (19.54x), indicating that this same proportional relationship likely applies to query performance as well. Were further work to be completed on this dictionary encoder, leveraging multithreading would be an obvious first step in improving the encoding performance. While performance with the largest file was acceptable, at just beyond three minutes, performance improvements would likely be a necessity for any column files in the 1 GB range and beyond. Overall, however, the dictionary encoder in its current state yielded consistent results, with moderate encoding performance and quite solid querying performance. While not formally tested for performance, the implemented decoding function also performed reliably, and confirmed the accuracy of the encoding process when comparing the decoder's output to the raw column files.
