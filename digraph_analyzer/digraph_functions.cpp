#include "digraph_functions.h"
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::sort;
using std::count;

/* ______read_txt______
 * read the contents of the text file with the name provided in the command line.
 * This function will read [1] 'n,' the number of digraphs/trigraphs to be processed,
 * [2] a list of 'n' digraphs/trigraphs, [3] the actual block of text into a vector.
 * The block of text will be read by making every character lowercase, removing punctuations,
 * and removing whitespaces (both space and newline).
 */
Fdata read_txt(string file_name) {
    // open file to read from
    ifstream to_read(file_name);

    // read 'n' from file
    int n;
    to_read >> n;

    // read the vector of digraphs from file
    vector<string> digraphs;
    string di;
    while((to_read.peek() != '\n') && (to_read >> di)) {
        digraphs.push_back(di);
    }

    // read text into a vector
    vector<string> words;
    string word;

    while(to_read >> word) {
        int len = word.size();
        // iterate chars in string to remove punctuation & make into lower case
        for (int i = 0; i < len; i++) {
            if (ispunct(word[i])) {
                word.erase(i--, 1);
                // prevent going out of bounds
                len = word.size(); // string length changes after punctuation is erased
            }

            // make into lower case
            word[i] = tolower(word[i]);
        }

        // add 'word' to the vector
        words.push_back(word);

        // break from loop when end of file is reached
        if (to_read.eof()) {
            break;
        }
    }

    // use self defined struct to return 3 variables
    Fdata to_return;
    to_return.n = n;
    to_return.di = digraphs;
    to_return.words = words;

    return to_return;
}



/* ______analyze_text______
 * takes Fdata read from 'read_txt' as a parameter and returns a Analyzed type struct 
 * variable that contains two maps. The first map 'num_occ' has each digraph/trigraph
 * as keys and has the integer number of occurances of each digraph as values. The
 * second map 'words_occ' also has the strings of digraphs as keys but has the string
 * vectors that contain the words that each digraph occurs in as values.
 */
Analyzed analyze_text(Fdata data) {
    // create map to count number of occurances for each digraph/trigraph
    map<string, int> map_occ;
    map<string, vector<string>> map_words;

    // fill in maps 'map_occ' and 'map_words'
    for (vector<string>::iterator it_d = (data.di).begin(); it_d != (data.di).end(); ++it_d) {
        map_occ.insert(pair<string, int>(*it_d, 0));
        map_words.insert(pair<string, vector<string>>(*it_d, {}));
    }

    // analyze data stored in 'data'
    for (vector<string>::iterator it_w = (data.words).begin(); it_w != (data.words).end(); ++it_w) {
        for (vector<string>::iterator it_d = (data.di).begin(); it_d != (data.di).end(); ++it_d) {
            if ((*it_w).find(*it_d) != std::string::npos) {
                map_occ[*it_d]++;
                map_words[*it_d].push_back(*it_w);
            }
        }
    }

    Analyzed to_return;
    to_return.num_occ = map_occ;
    to_return.words_occ = map_words;

    return to_return;
}



/* ______ascii______
 * print digraphs/trigraphs and the words they appear in the input text, ordered
 * in ASCII order (increasing from a to z).
 */
void ascii(Fdata fdata, Analyzed data) {
    vector<string> digraph = fdata.di;

    // sort in askii order (increasing from a to z)
    sort(digraph.begin(), digraph.end());

    // print digraphs & their occurances in the txt file using helper function
    print_sorted(data, digraph);
}



/* ______rev_ascii______
 * print digraphs/trigraphs and the words they appear in the input text, ordered
 * in reverse ASCII order (decreasing from z to a).
 */
void rev_ascii(Fdata fdata, Analyzed data) {
    vector<string> digraph = fdata.di;

    // sort in reverse askii order (decreasing from z to a)
    sort(digraph.rbegin(), digraph.rend());

    // print digraphs & their occurances using helper function
    print_sorted(data, digraph);
}



/* ______count______
 * print digraphs/trigraphs and the words they appear in the input text, ordered
 * from the digraph with the most occurances to the digraph with the least number
 * of occurances. If two digraphs have the same number of occurances, the digraph
 * with greater ASCII value is printed first. 
 */
void count(Analyzed data) {
    vector<pair<string, int>> map_to_vec;

    // copy map into vector of pairs
    for(map<string, int>::iterator it = (data.num_occ).begin(); it != (data.num_occ).end(); it++) {
        map_to_vec.push_back(make_pair(it->first, it->second));
    }

    // sort vector in decreasing order of num of occurance in the txt file
    sort(map_to_vec.begin(), map_to_vec.end(), [](const pair<string, int> &a, const pair<string, int> &b) {
        // compare ASCII values if number of occurances are equal for two different bigraph/trigraphs
        if(a.second == b.second) {
            if (a.first.compare(b.first) < 0) {
                return true;
            }
            else if (a.first.compare(b.first) > 0) {
                return false;
            }
        }
        return (b.second < a.second);
    });

    vector<string> order;

    // extract the keys from the vector sorted above into a new vector 
    for (unsigned long int i = 0; i < map_to_vec.size(); i++) {
        order.push_back(map_to_vec[(int)i].first);
    }

    // print digraphs & their occurances using helper function
    print_sorted(data, order);
}



/* ______print_sorted______
 * takes two parameters: an Analyzed type struct variable that contains two maps
 * and a string vector that contains the bigraphs/trigraphs that were analyzed in the
 * order to be printed in. Then, based on the order and format specified, this function prints
 * all bigraphs/trigraphs and their corresponding words found in the input txt file.
 */
void print_sorted(Analyzed data, vector<string> order) {
    // create temporary variable to store map containing words that each digraph/trigraph occurs in
    map<string, vector<string>> occ = data.words_occ;

    // print the bigraphs and their corresponding vector of words in the desired format & in the order specified by 'order'
    for (int i = 0; i < (int)order.size(); i++) {
        // print empty brackets if bigraph/trigraph has 0 occurances in the txt file
        if (occ[order[i]].empty()) {
            cout << order[i] << ": []" << endl;
        }
        else {
            cout << order[i] << ": [";
            for (int j = 0; j < (int)occ[order[i]].size(); j++) {
                // print right bracket if the word to be printed is the last word in the vector
                if (j == ((int)occ[order[i]].size() - 1)) {
                    cout << occ[order[i]][j] << "]" << endl;
                }
                else {
                    cout << occ[order[i]][j] << ", ";
                }
            }
        }
    }
}



/* ______print_number______
 * When user enters a number, list all the digraphs/trigraphs (in ASCII order) 
 * that occur that many times and their corresponding containing words (in order 
 * of their appearance in the text), or print None if none exists.
 */
void print_number(int n, Analyzed data) {
    int count = 0;
    // iterate over map to check if there are digraphs with 'n' occurances
    for  (map<string, int>::iterator it = (data.num_occ).begin(); it != (data.num_occ).end(); ++it) {
        if (it->second == n) {
            if (n == 0) {
                cout << it->first << ": []" <<endl;
            }
            else {
                cout << it->first << ": [";
                for (int i = 0; i < n; i ++) {
                    if (i == (n - 1)) {
                        cout << (data.words_occ[it->first])[i] << "]" << endl;
                    }
                    else {
                        cout << (data.words_occ[it->first])[i] << ", ";
                    }
                }
            }
            count++;
        }
    }
    if (count == 0) {
        cout << "None" << endl;
    }
}


/* ______print_digraph______
 * list how many times the digraph/trigraph occurs and in which words 
 * (in order of their appearance in the text) or "No such digraph" if it is 
 * not in the list of input digraphs/trigraphs. 0 would be printed for 
 * digraphs/trigraphs that are among the input list but not found in any word of the text.
 */
void print_digraph(string di, Fdata file, Analyzed data) {
    // make di case insensitive
    int len = di.size();
    for (int i = 0; i < len; i++) {
        di[i] = tolower(di[i]);
    }

    // if di not a valid digraph, print "No such digraph\n"
    if (!count((file.di).begin(), (file.di).end(), di)) {
        cout << "No such digraph" << endl;
    }
    else {
        if (data.num_occ[di] == 0) {
            cout << data.num_occ[di] << ": []" << endl;
        }
        else {
            cout << data.num_occ[di] << ": [";
            for (int j = 0; j < (int)(data.words_occ[di].size()); j++) {
                if (j == (int)(data.words_occ[di].size()) - 1) {
                    cout << (data.words_occ[di])[j] << "]" << endl;
                }
                else {
                    cout << (data.words_occ[di])[j] << ", ";
                }
            }
        }
    }
}


