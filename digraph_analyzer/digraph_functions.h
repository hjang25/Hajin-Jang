#ifndef DIGRAPH_FUNCTIONS_H
#define DIGRAPH_FUNCTIONS_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

/* struct to store data read from input file */
typedef struct _file_data {
    int n;
    std::vector<std::string> di;
    std::vector<std::string> words;
} Fdata;



/* struct to store data analyzed in two ways*/
typedef struct _analyzed_data {
    std::map<std::string, int> num_occ; // number of each digraph/trigraph's occurances
    std::map<std::string, std::vector<std::string>> words_occ; // words that each digraph/trigraph occurs in
} Analyzed;



/* ______read_txt______
 * read the contents of the text file with the name provided in the command line.
 * This function will read [1] 'n,' the number of digraphs/trigraphs to be processed,
 * [2] a vector of 'n' digraphs/trigraphs, [3] the actual block of text into a vector.
 * The block of text will be read by making every character lowercase, removing punctuations,
 * and removing whitespaces (both space and newline). This function will return a Fdata type
 * struct that stores the above three variables.
 */
Fdata read_txt(std::string file_name);



/* ______analyze_text______
 * takes Fdata read from 'read_txt' as a parameter and returns a Analyzed type struct 
 * variable that contains two maps. The first map 'num_occ' has each digraph/trigraph
 * as keys and has the integer number of occurances of each digraph as values. The
 * second map 'words_occ' also has the strings of digraphs as keys but has the string
 * vectors that contain the words that each digraph occurs in as values.
 */
Analyzed analyze_text(Fdata data);



/* ______ascii______
 * takes both Fdata and Analyzed type struct variables as parameters, sorts the string
 * vector stored in Fdata that contains all bigraphs/trigraphs to be analyzed in ASCII order, 
 * and uses the self-defined helper function 'print_sorted' to print the data stored in
 * the map in ascii order (increasing from a to z).
 */
void ascii(Fdata fdata, Analyzed data);



/* ______rev_ascii______
 * takes both Fdata and Analyzed type struct variables as parameters, sorts the string
 * vector stored in Fdata that contains all bigraphs/trigraphs to be analyzed in reverse 
 * ASCII order, and uses the self-defined helper function 'print_sorted' to print the data 
 * stored in the map in ascii order (decreasing from z to a).
 */
void rev_ascii(Fdata fdata, Analyzed data);



/* ______count______
 * takes a Analyzed type struct variable as a parameter, sorts the digraphs and their
 * corresponding number of occurances in order of most frequent to least frequent.
 * If two digraphs/trigraphs have the same number of occurance, it is considered that
 * the digraph/trigraph with a greater ASCII value is prioritized. This function then
 * prints data in the "c" mode using 'print_sorted.'
 */
void count(Analyzed data);



/* ______print_sorted______
 * takes two parameters: an Analyzed type struct variable that contains two maps
 * and a string vector that contains the bigraphs/trigraphs that were analyzed in the
 * order to be printed in. Then, based on the order and format specified, this function prints
 * all bigraphs/trigraphs and their corresponding words found in the input txt file.
 */
void print_sorted(Analyzed data, std::vector<std::string> order);



/* ______print_number______
 * takes two parameters: the user input of an integer and an Analyzed type struct variable.
 * This function lists/prints all the digraphs/trigraphs in ASCII order that occur 'n' times
 * and their corresponding containing words, in order of their appearance in the text.
 * If none exist, the program outputs "None."
 */ 
void print_number(int n, Analyzed data);



/* ______print_digraph______
 * takes 3 parameters: the user input of a string (it it does not equal "exit"), the Fdata type 
 * struct variable, and the Analyzed type struct variable. This function lists/prints the 
 * number of times the specified digraph/trigraph occurs and in which words they appear in the 
 * input text. This function prints "No such digraph" if the string does not match any of the 
 * digraphs/trigraphs provided in the input file. 0 is printed for digraphs/trigraphs that are 
 * among the input list but not found in any word of the text.
 */
void print_digraph(std::string di, Fdata file, Analyzed data);

#endif