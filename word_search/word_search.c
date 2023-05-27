// word_search.c
// 9804FA
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "search_functions.h"
#include "search_functions.c"
#define STR2(x) #x
#define STR(X) STR2(X)

/*
 * This is the HW3 main function that performs a word search.
 */
int main(int argc, char *argv[])
{
  char grid[MAX_SIZE][MAX_SIZE];
  char search_word[10];
  char lower_case;
  int dim;

  if (argc != 2) {
    printf("Please enter a command line argument.\n"); // Catching error
    return 1;
  }

  dim = populate_grid(grid, argv[1]);
  
  // Accepting user input of search words
  while(scanf(" %s", search_word) == 1) {
    // Making program case insensitive
    for(int i = 0; i < (int)strlen(search_word); i++) {
      lower_case = search_word[i];
      search_word[i] = tolower(lower_case);
    }

    find_all(grid, dim, search_word, stdout);
  }

  return 0;

}


