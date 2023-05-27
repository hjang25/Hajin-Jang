#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "digraph_functions.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::cerr;
using std::ofstream;
using std::ifstream;
using std::all_of;
using std::stoi;


int main(int argc, char * argv[]) {
  // if number of arguments is not correct, exit with code 1!
  if (argc != 3) {
    cout<<"Invalid arguments";
    return 1;
  }

  // if the "order" argument is not valid, exit with code 1!
  string order(argv[2]);
  if (order != "r" && order != "a" && order != "c") {
    cout<<"Invalid arguments";
    return 1;
  }

  // read input file
  string file_name = argv[1];
  Fdata txt_read = read_txt(file_name);

  // analyze data read from text file
  Analyzed txt_analyzed = analyze_text(txt_read);

  // sort & print analyzed data based on operation chosen by user
  string operation = argv[2];
  if (operation.compare("a") == 0) {
    ascii(txt_read, txt_analyzed);
  }
  else if (operation.compare("r") == 0) {
    rev_ascii(txt_read, txt_analyzed);
  }
  else {
    count(txt_analyzed);
  }

  string input;
  bool done = false;

  // loop until user inputs "exit"
  while (!done) {
    // prompt for user input
    cout << "q?>";
    cin >> input;

 
    // check if input is "exit"
    int comp = input.compare("exit");
    if (comp == 0) {
      done = true;
    }
    // check if input is a digit or string (other than "exit")
    else if (all_of(input.begin(), input.end(), isdigit)) {
      print_number(stoi(input), txt_analyzed);
    }
    else if (all_of(input.begin(), input.end(), isalpha)) {
      print_digraph(input, txt_read, txt_analyzed);
    }
    else {
      cout << "Invalid operation" << endl;
    }
  }
  return 0;
}


