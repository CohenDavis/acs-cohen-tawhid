#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <shared_mutex>
#define size 100

// Use linked list separate chaining to address collisions
class Node {
public:
	std::string value;
	std::string key;
	Node* next;
};

class Table {
public:
	Table() {
		for (int i = 0; i < size; i++) {
			dict[i] = NULL;
		}
		std::cout << "Initialized" << std::endl;
	}

	unsigned int hash_function(std::string& key) {
		//  http://www.partow.net/programming/hashfunctions/
	    //  This is a general-purpose, very good SAX hash function for strings.
		unsigned int hash = 1315423911;
	    for(unsigned int i = 0; i < key.length(); i++)
	      hash ^= ((hash << 5) + key[i] + (hash >> 2));
	    return hash%size;
	}

	void put(std::string key, std::string value) {
		// Find hash value and add new node to linked list if another key-value is already stored at this hash value.
		std::unique_lock lock(mutex_);

		unsigned int i = hash_function(key);
		//std::cout << i << std::endl;
		Node* tmp = dict[i];
		dict[i] = new Node;
		dict[i]->key = key;
		dict[i]->value = value;
		dict[i]->next = tmp;
	}

	void get(std::string key) {
		// Find hash value and go to this value in array and check linked list
		std::shared_lock lock(mutex_);

		unsigned int i = hash_function(key);
		Node* node = dict[i];
		while(node != NULL){
			if(node->key == key){
				std::cout << "Key: " << node->key << ", Value: " << node->value << std::endl;
				node = node->next;
			}
			else{
				node = node->next;
			}
		}
	}

	void print() {

		std::shared_lock lock(mutex_);

		for (int i = 0; i < current_size; i++) {
			Node* node = dict[i];
			while(node != NULL){
				std::cout << "Key: " << node->key << ", Value: " << node->value << std::endl;
				node = node->next;
			}

		}
	}

private:
	Node* dict[size];
	long current_size = size;
	mutable std::shared_mutex mutex_;
};


int main(int argc, char** argv){
	Table hash_table;

	hash_table.put("turtle", "green");
	hash_table.put("lion", "blue");
	hash_table.put("dog", "red");
	hash_table.put("dog", "purple");
	std::cout << "Finished Inserting" << std::endl;
	//hash_table.get("dog");
	//hash_table.print();

	//std::thread thread1(&Table::put, &hash_table, "dog", "green");
  //std::thread thread2(&Table::print, &hash_table);

  //thread1.join();
  //thread2.join();

	hash_table.print();
	return 0;
}
