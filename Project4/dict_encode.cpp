#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <x86intrin.h>
#include <map>

//Dictionary format: map(original string, pair(line of first string occurence, total number of occurences))
typedef std::map< std::string, std::pair<long long int, long long int> > DICT_TYPE;

//Generates/populates dictionary based on raw, unencoded file
void dict_gen_from_raw(std::ifstream& raw_data, DICT_TYPE& dictionary) {
  std::string data;
  DICT_TYPE::iterator it;
  long long int location = 0;

  //read raw data file and create dictionary
  while(raw_data >> data){

    it = dictionary.find(data);
    if (it == dictionary.end()) {
      dictionary.insert(std::make_pair(data, std::make_pair(location, 1)));
    }
    else {
      it->second.second += 1;
    }
    location += 1;

  }

}

//Writes dictionary and encoded data to file
void output_encoded(std::ifstream& raw_data, DICT_TYPE& dictionary) {
  std::ofstream outstream("encoded_file");
  DICT_TYPE::iterator it;
  it = dictionary.begin();

  //write dictionary to output file
  while (it != dictionary.end()){
    outstream << it -> first << " " << it -> second.first << " " << it -> second.second << std::endl;
    ++it;
  }
  outstream << ";" << std::endl; //use semicolon as delimiting character between dictionary and encoded data

  //write encoded data to output file
  std::string data;
  while(raw_data >> data){
    it = dictionary.find(data);
    if (it == dictionary.end()) {
      std::cout << "Encoded file write error" << std::endl;
    }
    else {
      outstream << it -> second.first << std::endl;
    }
  }
  outstream.close();

}

//Generates/populates dictionary based on dictonary data from encoded file
void dict_gen_from_encoded(std::ifstream& encoded_data, DICT_TYPE& dictionary) {
  std::string line

  while (!encoded_data.eof()) {
    std::getline(encoded_data, line);
    std::size_t end_check = line.find(';');
    //stop if find delimiting semicolon (indicating end of dictionary data)
    if (end_check != std::string::npos) {
      break;
    }
    else {
      //process line from encoded file, add data to dictionary
      std::size_t space_check = line.find(' '); //find first space
      std::string word substr(size_t pos = 0, space_check); //get raw "word"
      std::string remaining substr(space_check + 1); //get rest of line after word
      space_check = remaining.find(' '); //find next space
      long long int first_loc = stoi(substr(size_t pos = 0, space_check)); //get location string, convert to int
      long long int num_occur = stoi(substr(space_check + 1)); //get frquency string, convert to int

      dictionary.insert(std::make_pair(word, std::make_pair(first_loc, num_occur)));
    }
  }

}


int main(int argc, char** argv){
  char mode;
  std::string input_file;
  std::string input_item;

  std::cout << "Would you like to Encode (E), or Query (Q)?: ";
  std::cin >> mode;

  if (mode == 'E' || mode == 'e'){
    std::cout << "Enter the name of your raw/unencoded file: ";
    std::cin >> input_file;
  }
  else if (mode == 'Q' || mode == 'q') {
    std::cout << "Enter the name of your encoded file: ";
    std::cin >> input_file;
    std::cout << "Enter the data item (string) you'd like to search for: ";
    std::cin >> input_item;
  }
  else {
      std::cout << "Command Error" << std::endl;
      return 0;
  }

  std::ifstream instream(input_file);
  DICT_TYPE dictionary;
  if (mode == 'E' || mode == 'e'){
    dict_gen_from_raw(instream, dictionary);
    instream.close();
    std::ifstream instream(input_file);
    output_encoded(instream, dictionary);
    std::cout << "Encoded file generated" << std::endl;
  }
  else {
    dict_gen_from_encoded(instream, dictionary);
    instream.close();

    //Need to add actual query after dictionary generation - still need to test dict_gen_from_encoded
    //Should add decoding function - map(first loc, word) would be useful - need to add to dict_gen_from_encoded
    //SIMD stuff anywhere?


  }

  return 0;
}

/*
map(word, (first loc, num occur))
map(first loc, word)
*/
