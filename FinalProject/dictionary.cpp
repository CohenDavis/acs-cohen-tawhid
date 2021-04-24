#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <stdio.h>
#define size 100

// Use linked list separate chaining to address collisions
class Node {
public:
	std::string value;
	std::string key;
	Node* next;
};

unsigned int hash_function(std::string& key) {
	//  http://www.partow.net/programming/hashfunctions/ 
    //  This is a general-purpose, very good SAX hash function for strings.
	unsigned int hash = 1315423911;
    for(unsigned int i = 0; i < key.length(); i++)
      hash ^= ((hash << 5) + key[i] + (hash >> 2));
    return hash%size;
}

void put(Node* dict[size], std::string key, std::string value) {
	// Find hash value and add new node to linked list if another key-value is already stored at this hash value. 
	unsigned int i = hash_function(key);
	//std::cout << i << std::endl;
	Node* tmp = dict[i];
	dict[i] = new Node;
	dict[i]->key = key;
	dict[i]->value = value;
	dict[i]->next = tmp;
}

void get(Node* dict[size], std::string key) {
	// Find hash value and go to this value in array and check linked list 
	unsigned int i = hash_function(key);
	Node* node = dict[i];
	while(node != NULL){
		if(node->key = key){
			std::cout << "Key: " << node->key << ", Value: " << node->value << std::endl;
		}
		else{
			node = node->next;
		}
	}
}

void print(Node* dict[size]) {
	for (int i = 0; i < size; i++) {
		Node* node = dict[i];
		while(node != NULL){
			std::cout << "Key: " << node->key << ", Value: " << node->value << std::endl;
			node = node->next;
		}
		
	}
}

int main(int argc, char** argv){ 
	Node* dict[size];
	for (int i = 0; i < size; i++) {
		dict[i] = NULL;
	}
	std::cout << "Initialized" << std::endl;
	put(dict, "turtle", "green");
	put(dict, "lion", "blue");
	put(dict, "dog", "red");
	std::cout << "Finished Inserting" << std::endl;
	get(dict, "dog");
	//print(dict);
	return 0;
}