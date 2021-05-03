ACS Final Project: In-Memory Key-Value Store Implementation

# Brief Description
Per the professor's suggestion, the team will implement an in-memory key-value store system, leveraging either B-trees or hash tables for indexing. The implementation will at minimum support GET, PUT, and DELETE operations, as well as concurrency control.

# Homemade Hashtable
This program leverages a homemade hashtable to store the key value pairs. Therea are basically two layers to the hashtable. The top layer is a simple array of pointers and the second layer is a linked list each array index points to. The hash function will return value which will represent the index of the array. Each array index points to a linked list where each node contains the key-value pair. Linked List is used to address collisions:  if hash function returns same index value for two different items then second item is added to linked list after first item. 


