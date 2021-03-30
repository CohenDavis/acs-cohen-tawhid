Course Mini-Project #4: Implementation of Dictionary Encoder

# Overview

This program is designed to run in a Linux environment, using x86intrin.h for SIMD functionality. An Ubuntu bash terminal in Windows can also be used.

The code will then ask the user for two inputs. It will ask the user if they want to encode, query, or decode a file and then ask for which file to run the operation on.

# Encoding

This feature will convert a raw data file into a file that contains the raw data from the file and its location in the file. If the user selects to encode the file then a function will first read the raw data from the file into a dictinary. This dictionary utilizes the STD Map data structure which is an ordered associative data structure and allows O(logN) access time. The key value pair of this dictionary will be the data and location of this data. The next step in this process is to encode the dictionary into a file. This file will contain the data and location of this data stored in the dictionary. 

# Querying

This feature allows the user to look up a string data value in the encoded file. This function will take in the string data they want to look up and an encoded file. A function will first create a dictionary from the encoded file. The encoded file contains the string data and location of the data so it is simple to convert these two items from the file into a key value pair in order to populate the dictionary. This dictionary will also contain the frequency of how many times the data occurs in the file. Next the query feature will search this dictionary to determine if the string data given by the user exists in the file. If it exists then it will output how many times the string occur in the file. 

# Decoding

This feature will decode an encoded file into the original file it encoded from. It will first create a dictionary from the encoded file. The data and location of data in the encoded file will be used as key value pair of the dictionary. After this dictionary is populated, it will construct the original file. The original file will be created by placing the string data from the dictionary at it's correct location also given by the dictionary.  
