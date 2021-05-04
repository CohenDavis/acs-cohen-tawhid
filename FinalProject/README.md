ACS Final Project: In-Memory Key-Value Store Implementation

# Brief Description
Per the professor's suggestion, the team implemented an in-memory key-value store system, leveraging a hash table for indexing. The implementation supports GET, PUT, and (multiple) DELETE operations, as well as rehashing functionality and concurrency control. The following command can be used to compile and run the program. GCC version 6.0 or newer is required (testing was performed with v9.0), along with C++17.
```
 g++ -g dictionary.cpp -Wall -std=c++17 -lpthread -o dictionary.exe && ./dictionary.exe
```
If the user desires to experiment with the Table class (the key-value store implementation), the main function in "dictionary.cpp" can be modified to call any of the public methods (it is currently populated with test cases), or the Table class could be called from another file.

# Homemade Hash Table
This program leverages a homemade hash table to store the key-value pairs. There are basically two layers to the hash table. The top layer is a simple array of pointers and the second layer is a linked list each array index points to. The hash function will return a value which will represent the index of the array. Each array index points to a linked list where each node contains the key-value pair. Linked List is used to address collisions: if the  hash function returns the same index value for two different items then the second item is added to the linked list after the first item. This hash table and other relevant variables are contained in and managed by a Table class.

![hashtable](https://github.com/CohenDavis/acs-cohen-tawhid/blob/master/FinalProject/hashtable.PNG)

# Hash Function

The hash function will take in a key, which is a string, and then return an unsigned integer corresponding to the index in the hash table array. The SAX hash function is used because it returns a uniform and randomlike value after hashing. We hash the key string and then do a modulo division by size of the hash table array so we can return a proper index in the array. The hash function takes in an integer (desired_size) corresponding to the size of the target hash table array, so that the hash function easily scales with tables of varying sizes.

```
unsigned int hash_function(std::string& key, long desired_size) {
		//  http://www.partow.net/programming/hashfunctions/
	    //  This is a general-purpose, very good SAX hash function for strings.
		unsigned int hash = 1315423911;
	    for(unsigned int i = 0; i < key.length(); i++)
	      hash ^= ((hash << 5) + key[i] + (hash >> 2));
	    return hash%desired_size;
	}
```

# Put and Get Functions

The Put function first hashes the key of the key-value pair. It then creates a new linked list node containing the key and value. This node is then added to the linked list associated with the array index given by the hashed key in the beginning. Each node also has a "next" pointer which is accordingly set to point to the node which was previously at the front of that index's linked list (or "next" is set to NULL if there are no nodes present yet).

![hashtable](https://github.com/CohenDavis/acs-cohen-tawhid/blob/master/FinalProject/insert.PNG)

The Get function is similar to the Put function, but the node already exists. The input is a key string. This key is hashed to get an index value then the linked list associated with this hashed index is searched. The node containing the actual key-value is then found and its value is returned. If a key corresponds to multiple values, all values are returned.

# Delete Functions

In developing the deletion functionality for this key-value store implementation, two variations of delete functions were produced. Due to how the Table class and its Put method are currently configured, a single key can possess multiple values. Thus, a user of this Table class may want to remove all the values for a key (and the key itself), or just a single value for that key. In an effort to accommodate this, the first method implemented was a Delete Key (delete_key()) method, which takes the key desired for deletion as an argument. When called on a Table object and provided with a key, this method first hashes the input key to find the appropriate hash table index where desired key-value pairs would be located. The first node at that index will then be checked for a matching key, and if a match is found, the initial pointer for the index is redirected to the second node (or NULL if there is no second node), and the first node is deleted. If there is no match found on the first node and there is more than one node at that index (i.e. - if two different keys were to produce the same hash), the nodes' "next" pointers are used for the remaining checks. For example, it will be evaluated if node->next's key matches, and if so, node's next pointer will be redirected to node->next->next, and the old node->next will be deleted. This "look ahead" method is easier due to the lack of a "prev" pointer in this singly-linked list implementation.

![hashtable](https://github.com/CohenDavis/acs-cohen-tawhid/blob/master/FinalProject/deletion.png)

Once all nodes at the proper index have been evaluated, the method finishes and returns (except in cases where a rehash is desired, which will be discussed later). The second delete function variation is the Delete Pair (delete_pair()) method, which takes both a desired key and a desired value as arguments. This method functions very similarly to Delete Key, with the exception of some of the checking conditions when evaluating nodes for potential deletion. Instead of deleting all nodes with the same key (deleting all of a key's values), Delete Pair deletes a specific node with a specific key for a particular value (deleting one of a key's values). For instances of the Table class where keys only have a single distinct value, Delete Key and Delete Pair will exhibit the same behavior.

# Rehash Functions

In addition to the fundamental Get, Put, and Delete methods, rehashing functionality was implemented to allow for expansion or contraction of the table depending on how much data a user is handling at a given moment. Theoretically, this helps strike a balance between time and space/memory/storage costs. As part of this rehashing implementation, two rehashing methods were introduced. The first, rehash_larger(), is called whenever the table's load factor (number of key-value pairs divided by the size of the table) exceeds 0.75. This load factor check occurs at the end of the Put function, meaning the check occurs whenever any key-value pair is added. When called, this rehash_larger() method creates a new table with a size 50 indices larger than the current/previous table, then rehashes the currently stored keys and transfers the appropriate data over to the new table (with the new table being assigned to the appropriate private member variable and the old table being deleted upon completion). The second rehashing method, rehash_smaller(), is called whenever the table's load factor dips below 0.25 and the table has previously been expanded beyond the starting 100 index size. This additional check that the table has previously been expanded prevents unnecessary calls to rehash_smaller() when data is first being added to the table and the load factor is still low. The rehash_smaller() method works in the same way as rehash_larger(), with the key difference being that the table is shrunk by 50 indices rather than expanded. The load factor check for this rehashing function is performed at the end of both the Delete Key and Delete Pair methods (whenever any node might be deleted). While this expansion and contraction by 50 indices works well for small and medium sized data sets, this size change would likely need to be increased for data sets on the scale of several thousand key-value pairs and beyond. This is because of how changes of 50 indices will affect the load factor a decreasingly small amount as data set size increases (i.e. 0.75 to 0.73).

# Concurrency Control

In implementing concurrency control for the Table class, the std::shared_mutex library was leveraged to restrict simultaneous access to specific methods by concurrent threads/transactions. As previously mentioned, this library necessitated v6.0+ of the GCC compiler (v9.0 was tested here), and C++17. Implementation of functionality from this shared mutex library was fairly straightforward, requiring a mutex declaration in the private member variables of the Table class, along with locking statements in the methods where locking is desired. More specifically, shared locks (std::shared_lock - effectively read locks) were added to the Get and Print methods. More than one thread can hold a shared lock at one time, thus allowing more than one read of the table to occur concurrently. Additionally, exclusive locks (std::unique_lock - effectively write locks) were added to the Put, Delete Key, and Delete Pair methods, restricting access to the Table object to whichever thread holds the lock during the execution of those functions. For both of these lock types, the locks are released (unlocking) upon completion/the return of the locked function. Helper functions like the hash function do not necessitate a lock due to the lack of interaction with the hash table itself and the data therein. It is also worth noting that each thread can only hold one lock at a time. As a result, the rehashing functions previously detailed do not require locks despite involving heavy manipulation of the hash table structure (deadlocking issues could result otherwise). However, the only functions that call those methods (Put, Delete Key, and Delete Pair) all require unique locks, meaning that the rehashing functions are still effectively locked in a "transitive" sort of manner.

# Verification/Testing

### Hardware Used During Testing

Dell XPS-9560:
* i7-7700HQ (4C/8T, 2.80GHz Base/3.80GHz Boost)
* GTX 1050 (2GB GDDR5)
* 16GB RAM
* 512 GB NVMe SSD
* Windows 10 64-bit

### Testing Methodology & Outcome

Upon completion of the key-value store infrastructure and methods, the main function of dictionary.cpp was then populated with multiple Table instantiations and threads/transactions (std::threads). Those threads were then fed lambda functions which contain various series of operations on those Table instantiations. The first test performed could be called the "Larger Put Test". Here, 80 Put operations on one Table object were split evenly between two threads. Those threads were run concurrently and then rejoined, with the final contents of the table then being printed. This test serves to demonstrate the proper behavior of both the Put function and the exclusive locking mechanism when faced with a (relatively) high number of successive and concurrent calls. Additionally, the number of key-value pairs added here is high enough to trigger a rehash via rehash_larger(), allowing for the verification of that table restructuring functionality. The final size of the table was also printed to make this verification easier. The time taken for the two threads to execute was also calculated, and this was reported as 0.0000000 seconds, or somewhere below 100 ns. Thus, while the group doesn't have data on how the implementation scales to the 1000+ key-value pair range, writes to and expansions of the table are notably quick with small to medium amounts of input data.

After the "Larger Put Test", the next test performed could be considered the "Larger Delete Test". Here, 60 Delete Key operations were performed via one thread on the same Table object previously constructed in the "Larger Put Test". Again, final contents of the table were printed to verify proper behavior. In this case, the 60 deletions brought the table's load factor below the 0.25 threshold, triggering another downsize and rehash via the rehash_smaller() method. As the table's size should have increased to 150 during the "Larger Put Test", its size should have dropped back down to 100 here (and this was indeed observed). As observed with the previous test, the time taken to perform the "Larger Delete Test" was again reported as 0.0000000 seconds.

The final test performed could be called the "Smaller Mixed Test". Here, the emphasis wasn't on pushing as much data as possible through threads and into the table, but rather on simple verification of all primary methods with a small amount of data. Only 11 operations were performed here, with various calls to Put, Get, Delete Key, and Delete Pair on a fresh Table object via 2 concurrent threads. Put, Delete Key, and the locking mechanisms were previously checked, but they get a final verification here. Get, despite being a short and simple function, gets more than one call here to verify the proper fetching and output of key-value data (including one case with a single value to a key, and one case with multiple values to a key). Delete Key and Delete Pair are also both called here on separate keys, with multiple values respectively, to help highlight and verify their differences. Ultimately, with this test, along with the previously described tests, the final table contents yielded aligned with expectations, and execution times (for the modest in scope testing data) were near-instantaneous.

# Conclusions

Overall, implementation of this key-value store via hash table proceeded as expected. The Get, Put, and Delete operations produced the desired effects on instantiations of the Table class during testing, and printed outputs served to verify this. Due to the integration of the shared mutex library and the associated locking mechanisms, these functions can also be called concurrently by multiple threads and still produce the desired results without "breaking" the data structure. The rehashing functions also allow Table objects to maintain a balance between time and space costs. Were further work to be done on this key-value store system, it would be beneficial to perform additional/extended testing. While the testing completed was able to verify the general functionality and performance of the Table class and its methods, it would be beneficial to do further testing of edge cases given additional time. Aside from this, the implementation could be expanded further with features like logging, or more advanced multi-version concurrency control. In its current state, however, the Table class achieves much of the core functionality of a key-value store system, and performs both reliably and efficiently.
