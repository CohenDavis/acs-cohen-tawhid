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
		dict = new Node* [size];
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
		num_items += 1;
		//determine if rehash necessary
		if (float(num_items)/float(current_table_size) > 0.75) {
			rehash_larger();
		}
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

		for (int i = 0; i < current_table_size; i++) {
			Node* node = dict[i];
			while(node != NULL){
				std::cout << "Key: " << node->key << ", Value: " << node->value << std::endl;
				node = node->next;
			}

		}
	}

	void delete_key(std::string key) {

				std::unique_lock lock(mutex_);

				unsigned int i = hash_function(key);
				Node* node = dict[i];
				long loop_num = 0;
				while(node != NULL){

					if(node->key == key && loop_num == 0){ //first pair matches, need to delete
						dict[i] = node->next;
						delete node;
						num_items -= 1;
						node = dict[i];
					}
					else if (node->next == NULL) { //if there is no next pair
						break;
					}
					else if(node->next->key == key) { //if there is a next pair, and its key matches
						Node* temp = node->next;
						node->next = node->next->next;
						num_items -= 1;
						delete temp;
						node = node->next;
					}
					else{ //no match
						loop_num += 1;
						node = node->next;
					}
				}

	}

	void delete_pair(std::string key, std::string value) {

				std::unique_lock lock(mutex_);

				unsigned int i = hash_function(key);
				Node* node = dict[i];
				long loop_num = 0;
				while(node != NULL){

					if(node->key == key && node->value == value && loop_num == 0){  //first pair matches, need to delete
						dict[i] = node->next;
						delete node;
						num_items -= 1;
						node = dict[i];
					}
					else if (node->next == NULL) { //if there is no next pair
						break;
					}
					else if(node->next->key == key && node->next->value == value) { //if there is a next pair, and its key matches
						Node* temp = node->next;
						node->next = node->next->next;
						num_items -= 1;
						delete temp;
						node = node->next;
					}
					else{ //no match
						loop_num += 1;
						node = node->next;
					}
				}

	}

	void rehash_larger() {
		//initialize temporary table
		Node ** temp_dict = new Node*[current_table_size + 50];
		for (int i = 0; i < (current_table_size+50); i++) {
			temp_dict[i] = NULL;
		}
		//copy data from current table over to larger temp table
		for (int i = 0; i < current_table_size; i++) {
			Node* node = dict[i];
			while(node != NULL){
				unsigned int j = hash_function(node->key);

				Node* temp_node = temp_dict[j];
				temp_dict[j] = new Node;
				temp_dict[j]->key = node->key;
				temp_dict[j]->value = node->value;
				temp_dict[j]->next = temp_node;
				node = node->next;
			}
		}
		//replace the current table with the temporary table
		for (int i = 0; i < current_table_size; i++) {
			delete dict[i];
		}
		dict = temp_dict;
		current_table_size += 50;
	}

	void print_table_size(){
		std::cout << "Table is " << current_table_size << std::endl;
	}

private:
	Node** dict;
	long current_table_size = size;
	long num_items = 0;
	mutable std::shared_mutex mutex_;
};


int main(int argc, char** argv){
	Table hash_table;

	hash_table.put("turtle", "green");
	hash_table.put("lion", "blue");
	hash_table.put("dog", "red");
	hash_table.put("dog", "purple");
	std::cout << "Finished Inserting" << std::endl;
	hash_table.put("dog", "orange");
	hash_table.put("lion", "orange");
	hash_table.print();
	hash_table.print_table_size();
	hash_table.delete_key("lion");
	hash_table.print();
	hash_table.print_table_size();
	hash_table.delete_pair("dog", "orange");
	hash_table.print();


	//hash_table.get("dog");
	//hash_table.print();

	//std::thread thread1(&Table::put, &hash_table, "dog", "green");
  //std::thread thread2(&Table::print, &hash_table);

  //thread1.join();
  //thread2.join();

	return 0;
}
