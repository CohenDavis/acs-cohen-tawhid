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
	//Constructor
	Table() {
		dict = new Node* [size];
		for (int i = 0; i < size; i++) {
			dict[i] = NULL;
		}
		std::cout << "Initialized" << std::endl;
	}

	//Hash function
	unsigned int hash_function(std::string& key, long desired_size) {
		//  http://www.partow.net/programming/hashfunctions/
	    //  This is a general-purpose, very good SAX hash function for strings.
		unsigned int hash = 1315423911;
	    for(unsigned int i = 0; i < key.length(); i++)
	      hash ^= ((hash << 5) + key[i] + (hash >> 2));
	    return hash%desired_size;
	}

	//Put function - adds key-value pair to table - requires an exclusive lock
	void put(std::string key, std::string value) {
		// Find hash value and add new node to linked list if another key-value is already stored at this hash value.
		std::unique_lock lock(mutex_);

		unsigned int i = hash_function(key, current_table_size);
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

	//Get function - returns all values stored for a key - requires a shared lock
	void get(std::string key) {
		// Find hash value and go to this value in array and check linked list
		std::shared_lock lock(mutex_);

		unsigned int i = hash_function(key, current_table_size);
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

	//Print function - prints all contents (key-value pairs) of table - requires a shared lock
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

	//Deletes all values for a particular key - requires an exclusive lock
	void delete_key(std::string key) {

				std::unique_lock lock(mutex_);

				unsigned int i = hash_function(key, current_table_size);
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
			//determine if rehash necessary
			if (float(num_items)/float(current_table_size) < 0.25 && current_table_size > 100) {
				rehash_smaller();
			}

	}

	//Deletes a specific value for a particular key - requires an exclusive lock
	void delete_pair(std::string key, std::string value) {

				std::unique_lock lock(mutex_);

				unsigned int i = hash_function(key, current_table_size);
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
			//determine if rehash necessary
			if (float(num_items)/float(current_table_size) < 0.25 && current_table_size > 100) {
				rehash_smaller();
			}

	}

	//Increases table size by 50 if load factor exceeds 0.75
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
				unsigned int j = hash_function(node->key, (current_table_size+50));

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

	//Decreases table size by 50 if load factor below 0.25
	void rehash_smaller() {
		//initialize temporary table
		Node ** temp_dict = new Node*[current_table_size - 50];
		for (int i = 0; i < (current_table_size-50); i++) {
			temp_dict[i] = NULL;
		}
		//copy data from current table over to smaller temp table
		for (int i = 0; i < current_table_size; i++) {
			Node* node = dict[i];
			while(node != NULL){
				unsigned int j = hash_function(node->key, (current_table_size-50));

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
		current_table_size -= 50;

	}

	//Prints number of indices in table - more for dev purposes
	void print_table_size(){
		std::cout << "Table is " << current_table_size << std::endl;
	}


private:
	Node** dict; //declare table object (dynamically)
	long current_table_size = size; //number of indices in table
	long num_items = 0; //number of nodes (key-value pairs) in table
	mutable std::shared_mutex mutex_; //mutex for concurrency control
};

//Main function used for testing the key-value store implementation
int main(int argc, char** argv){

	Table hash_table; //Instantiate key-value store table
	Table hash_table_2;

	//Lambda function for testing
	auto transaction_1 = [&hash_table]() {
		hash_table.put("01", "red");
		hash_table.put("02", "red");
		hash_table.put("03", "red");
		hash_table.put("04", "red");
		hash_table.put("05", "red");
		hash_table.put("06", "red");
		hash_table.put("07", "red");
		hash_table.put("08", "red");
		hash_table.put("09", "red");
		hash_table.put("10", "red");
		hash_table.put("11", "red");
		hash_table.put("12", "red");
		hash_table.put("13", "red");
		hash_table.put("14", "red");
		hash_table.put("15", "red");
		hash_table.put("16", "red");
		hash_table.put("17", "red");
		hash_table.put("18", "red");
		hash_table.put("19", "red");
		hash_table.put("20", "red");
		hash_table.put("21", "red");
		hash_table.put("22", "red");
		hash_table.put("23", "red");
		hash_table.put("24", "red");
		hash_table.put("25", "red");
		hash_table.put("26", "red");
		hash_table.put("27", "red");
		hash_table.put("28", "red");
		hash_table.put("29", "red");
		hash_table.put("30", "red");
		hash_table.put("31", "red");
		hash_table.put("32", "red");
		hash_table.put("33", "red");
		hash_table.put("34", "red");
		hash_table.put("35", "red");
		hash_table.put("36", "red");
		hash_table.put("37", "red");
		hash_table.put("38", "red");
		hash_table.put("39", "red");
		hash_table.put("40", "red");
	};
	//Lambda function for testing
	auto transaction_2 = [&hash_table]() {
		hash_table.put("41", "red");
		hash_table.put("42", "red");
		hash_table.put("43", "red");
		hash_table.put("44", "red");
		hash_table.put("45", "red");
		hash_table.put("46", "red");
		hash_table.put("47", "red");
		hash_table.put("48", "red");
		hash_table.put("49", "red");
		hash_table.put("50", "red");
		hash_table.put("51", "red");
		hash_table.put("52", "red");
		hash_table.put("53", "red");
		hash_table.put("54", "red");
		hash_table.put("55", "red");
		hash_table.put("56", "red");
		hash_table.put("57", "red");
		hash_table.put("58", "red");
		hash_table.put("59", "red");
		hash_table.put("60", "red");
		hash_table.put("61", "red");
		hash_table.put("62", "red");
		hash_table.put("63", "red");
		hash_table.put("64", "red");
		hash_table.put("65", "red");
		hash_table.put("66", "red");
		hash_table.put("67", "red");
		hash_table.put("68", "red");
		hash_table.put("69", "red");
		hash_table.put("70", "red");
		hash_table.put("71", "red");
		hash_table.put("72", "red");
		hash_table.put("73", "red");
		hash_table.put("74", "red");
		hash_table.put("75", "red");
		hash_table.put("76", "red");
		hash_table.put("77", "red");
		hash_table.put("78", "red");
		hash_table.put("79", "red");
		hash_table.put("80", "red");
	};

	//Lambda function for testing
	auto transaction_3 = [&hash_table]() {
		hash_table.delete_key("01");
		hash_table.delete_key("02");
		hash_table.delete_key("03");
		hash_table.delete_key("04");
		hash_table.delete_key("05");
		hash_table.delete_key("06");
		hash_table.delete_key("07");
		hash_table.delete_key("08");
		hash_table.delete_key("09");
		hash_table.delete_key("10");
		hash_table.delete_key("11");
		hash_table.delete_key("12");
		hash_table.delete_key("13");
		hash_table.delete_key("14");
		hash_table.delete_key("15");
		hash_table.delete_key("16");
		hash_table.delete_key("17");
		hash_table.delete_key("18");
		hash_table.delete_key("19");
		hash_table.delete_key("20");
		hash_table.delete_key("21");
		hash_table.delete_key("22");
		hash_table.delete_key("23");
		hash_table.delete_key("24");
		hash_table.delete_key("25");
		hash_table.delete_key("26");
		hash_table.delete_key("27");
		hash_table.delete_key("28");
		hash_table.delete_key("29");
		hash_table.delete_key("30");
		hash_table.delete_key("31");
		hash_table.delete_key("32");
		hash_table.delete_key("33");
		hash_table.delete_key("34");
		hash_table.delete_key("35");
		hash_table.delete_key("36");
		hash_table.delete_key("37");
		hash_table.delete_key("38");
		hash_table.delete_key("39");
		hash_table.delete_key("40");
		hash_table.delete_key("41");
		hash_table.delete_key("42");
		hash_table.delete_key("43");
		hash_table.delete_key("44");
		hash_table.delete_key("45");
		hash_table.delete_key("46");
		hash_table.delete_key("47");
		hash_table.delete_key("48");
		hash_table.delete_key("49");
		hash_table.delete_key("50");
		hash_table.delete_key("51");
		hash_table.delete_key("52");
		hash_table.delete_key("53");
		hash_table.delete_key("54");
		hash_table.delete_key("55");
		hash_table.delete_key("56");
		hash_table.delete_key("57");
		hash_table.delete_key("58");
		hash_table.delete_key("59");
		hash_table.delete_key("60");
	};

	//Lambda function for testing
	auto transaction_4 = [&hash_table_2]() {
			hash_table_2.put("turtle", "green");
			hash_table_2.put("turtle", "blue");
			hash_table_2.get("turtle");
			hash_table_2.delete_pair("turtle", "green");
			hash_table_2.put("goat", "orange");
		};

	//Lambda function for testing
	auto transaction_5 = [&hash_table_2]() {
			hash_table_2.put("lion", "blue");
			hash_table_2.put("dog", "red");
			hash_table_2.put("dog", "pink");
			hash_table_2.get("turtle");
			hash_table_2.get("lion");
			hash_table_2.delete_key("dog");
			};


	//Performs 80 put operations on one table, load factor should be exceeded, expanding table
	std::cout << "\nLarge Put Test..." << std::endl;
	clock_t Time1 = clock();
	std::thread thread_1(transaction_1);
	std::thread thread_2(transaction_2);
	thread_1.join();
	thread_2.join();
	clock_t Time2 = clock();
	float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC; //calculate time taken to complete

	//Verifies 80 puts + table size
	std::cout << "Table Contents:" << std::endl;
	hash_table.print();
	hash_table.print_table_size();
	printf("Time taken to complete 80 puts (2 threads) is %.7f \n", TotalTimeLoop);

	//Performs 60 deletions, should drop below minimum load factor, shrinking table
	std::cout << "\nLarge Delete Test..." << std::endl;
	Time1 = clock();
	std::thread thread_3(transaction_3);
	thread_3.join();
	Time2 = clock();
	TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC; //calculate time taken to complete
	//Verifies 60 deletions + table size
	std::cout << "Table Contents:" << std::endl;
	hash_table.print();
	hash_table.print_table_size();
	printf("Time taken to complete 60 deletions (1 thread) is %.7f \n", TotalTimeLoop);

	//General mixed operations, multiple transactions/threads test
	std::cout << "\nMixed Operations Test..." << std::endl;
	std::thread thread_4(transaction_4);
	std::thread thread_5(transaction_5);
	thread_4.join();
	thread_5.join();
	//Verify table contents
	std::cout << "Table Contents:" << std::endl;
	hash_table_2.print();


	return 0;
}
