#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
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

//Writes dictionary and encoded data to file "encoded_file"
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
void dict_gen_from_encoded(std::ifstream& encoded_data, DICT_TYPE& dictionary, std::map<long long int, std::string>& dict_supp) {
  std::string line;

  while (!encoded_data.eof()) {
    std::getline(encoded_data, line);
    std::size_t end_check = line.find(';');
    //stop if find delimiting semicolon (indicating end of dictionary data)
    if (end_check != std::string::npos) {
      break;
    }
    else {
      //process line from encoded file, add data to dictionary
      std::size_t pos = 0;
      std::size_t space_check = line.find(' '); //find first space
      std::string word = line.substr(pos, space_check); //get raw "word"
      std::string remaining = line.substr(space_check + 1); //get rest of line after word
      space_check = remaining.find(' '); //find next space
      long long int first_loc = stoi(remaining.substr(pos, space_check)); //get location string, convert to int
      long long int num_occur = stoi(remaining.substr(space_check + 1)); //get frequency string, convert to int

      dictionary.insert(std::make_pair(word, std::make_pair(first_loc, num_occur)));
      dict_supp.insert(std::make_pair(first_loc, word)); //helper structure to make decoding easier
    }
  }

}

//Writes decoded data to file "decoded_file"
void output_decoded(std::ifstream& encoded_data, std::map<long long int, std::string>& dict_supp) {
  std::ofstream outstream("decoded_file");
  std::map<long long int, std::string>::iterator it;
  std::string line;

  //navigate to encoded data part of file
  while (!encoded_data.eof()) {
    std::getline(encoded_data, line);
    std::size_t end_check = line.find(';');
    //look for delimiting semicolon (indicating end of dictionary data)
    if (end_check == std::string::npos) {
      continue;
    }
    else {
      break;
    }
  }
  //perform decoding on encoded data in file
  long long int code;
  while (encoded_data >> code) {
    it = dict_supp.find(code);
    outstream << it -> second << std::endl;
  }
  outstream.close();

}

int main(int argc, char** argv){
  char mode;
  std::string input_file;
  std::string input_item;

  std::cout << "Would you like to Encode (E), or Query (Q)?, or Decode (D): ";
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
  else if (mode == 'D' || mode == 'd'){
    std::cout << "Enter the name of your encoded file: ";
    std::cin >> input_file;
  }
  else {
      std::cout << "Command Error" << std::endl;
      return 0;
  }

  std::ifstream instream(input_file);
  DICT_TYPE dictionary;
  std::map<long long int, std::string> dict_supp;

  if (mode == 'E' || mode == 'e'){ //encoding functionality

    clock_t Time1 = clock();
    dict_gen_from_raw(instream, dictionary); //generate dictionary
    instream.close();
    std::ifstream instream(input_file);
    output_encoded(instream, dictionary); //use dictionary to convert raw file to encoded
    clock_t Time2 = clock();
    std::cout << "Encoded file generated" << std::endl;
    float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC; //calculate time taken to complete encoding
    printf("Time taken to complete encoding is %.7f \n", TotalTimeLoop);

  }
  else if (mode == 'Q' || mode == 'q'){ //querying functionality

    clock_t Time1 = clock();
    dict_gen_from_encoded(instream, dictionary, dict_supp); //generate dictionary
    /* Note - dict_supp is not useful here, and is only used for the
    decoding functionality - it is present, however, to facilitate the reuse
    of the dictionary generation function between the querying and decoding modes */
    instream.close();

    DICT_TYPE::iterator it = dictionary.find(input_item); //perform query on dictionary
    if (it == dictionary.end()) {
      std::cout << "The item you entered is not in the provided file" << std::endl;
    }
    else {
      std::cout << it -> first << " occurs " << it -> second.second << " times in the provided file" << std::endl;
    }

    clock_t Time2 = clock();
    float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC; //calculate time taken to complete query
    printf("Time taken to complete query is %.7f \n", TotalTimeLoop);

  }
  else { //decoding functionality

    clock_t Time1 = clock();
    dict_gen_from_encoded(instream, dictionary, dict_supp); //generate dictionary
    instream.close();
    std::ifstream instream(input_file);
    output_decoded(instream, dict_supp); //use dictionary data to convert encoded data back to raw
    clock_t Time2 = clock();
    std::cout << "Decoded file generated" << std::endl;
    float TotalTimeLoop = ((float) Time2 - (float) Time1) / CLOCKS_PER_SEC; //calculate time taken to complete decoding
    printf("Time taken to complete decoding is %.7f \n", TotalTimeLoop);

  }

  return 0;
}
