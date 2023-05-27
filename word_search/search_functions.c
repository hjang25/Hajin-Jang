// search_functions.c
// <STUDENT: ADD YOUR INFO HERE: name, JHED, etc.>
//
//


#include <stdio.h>
#include "search_functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/* 
 * Given a filename and a MAX_SIZExMAX_SIZE grid to fill, this function 
 * populates the grid and returns n, the actual grid dimension. 
 * If filename_to_read_from can't be opened, this function returns -1.
 * If the file contains an invalid grid, this function returns -2.
 */
int populate_grid(char grid[][MAX_SIZE], char filename_to_read_from[]){
  FILE *grid_file = fopen(filename_to_read_from, "r");
  int row = 0, col = 0, counter = 0, letters_column[10] = {0};
  char c;

  if(grid_file == NULL) {
    printf("Grid file failed to open."); 
    return -1;
  }

  while (!feof(grid_file) && (c = fgetc(grid_file)))
  {
    if (c == '\n')
    {
      letters_column[row] = col;
      row++;
      col = 0;
    }
    if (c != '\r' && c != '\n')
    {
      grid[row][col] = c;
      col++;
    }
  }

  // Below two lines are necessary to keep row, column values accurate
  col--;
  row++;

  // Since last line does not have a newline, calculate how many letters are in last line of grid
  for(int i = 0; i < MAX_SIZE; i++) {
    if(isalpha(grid[col-1][i]) > 0) {
      counter++;
    }
  }
  letters_column[col-1] = counter;

  fclose(grid_file);

  for(int i = 0; i < row; i++) {
    if(letters_column[0] != letters_column[i]) {
      printf("Invalid grid.\n"); 
      return -2;
    }

    if(row != col) {
      printf("Invalid grid.\n"); 
      return -2;
    }

    if(letters_column[i] == 0 || letters_column[i] > MAX_SIZE) {
      printf("Invalid grid.\n"); 
      return -2;
    }
  }
  if(row == 0 || row > MAX_SIZE) {
    printf("Invalid grid.\n"); 
    return -2;
  }
  return row; // If grid is valid, return grid dimension

}

/*
 * This function returns the number of times the given 
 * word string was found in the grid facing right. 
 * Parameter n indicates the actual size of the grid (nxn).
 * The function sends corresponding output to the specified 
 * file pointer, which already points to an open stream. Output lines 
 * must appear in order of the first character's appearance in a 
 * left-to-right scan of each row beginning with row 0.
 */
int find_right(char grid[][MAX_SIZE], int n, char word[], FILE *write_to){
  bool match = false;
  int counter = 0;
  
  // Comparing letters to search word
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
        if(grid[i][j] == word[0]) {
          for(int x = 1; x < (int)strlen(word); x++) {
            if (i + x < MAX_SIZE) {
              if(grid[i][j + x] == word[x]) {
                match = true;
              }
              else {
                match = false;
                break;
              }
            }
          }
          if(match == true) {
            fprintf(write_to, "%s %d %d R\n", word, i, j);
            counter++;
          }
        }
      }
    }
  
  return counter;
}


/*
 * This function returns the number of times the given 
 * word string was found in the grid facing left. 
 * Parameter n indicates the actual size of the grid (nxn).
 * The function sends corresponding output to the specified 
 * file pointer, which already points to an open stream. Output lines 
 * must appear in order of the first character's appearance in a 
 * left-to-right scan of each row beginning with row 0.
 */
int find_left (char grid[][MAX_SIZE], int n, char word[], FILE *write_to){
  int i, len, tempo; 
  char *rev_word; 
  bool match = false;
  int counter = 0;

  rev_word = (char*)malloc(10);

  strcpy(rev_word, word);
  
  len = (int)strlen(word); 
    
  // Iterating over string to reverse order
  for (i = 0; i < len/2; i++)  
  {  
    tempo = rev_word[i]; // temporary variable to hold the char
    rev_word[i] = rev_word[len - i - 1];  
    rev_word[len - i - 1] = tempo;  
  }
  
  // Comparing letters to search word
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      if(grid[i][j] == rev_word[0]) {
        for(int x = 1; x < (int)strlen(rev_word); x++) {
          if (i + x < MAX_SIZE) {
            if(grid[i][j + x] == rev_word[x]) {
              match = true;
             }
            else {
              match = false;
              break;
            }
          }
        }
        if(match == true) {
          fprintf(write_to, "%s %d %d L\n", word, i, j);
          counter++;
        }
      }
    }
  }

  return counter;

}


/*
 * This function returns the number of times the given 
 * word string was found in the grid facing down. 
 * Parameter n indicates the actual size of the grid (nxn).
 * The function sends corresponding output to the specified 
 * file pointer, which already points to an open stream. Output lines 
 * must appear in order of the first character's appearance in a 
 * left-to-right scan of each row beginning with row 0.
 */
int find_down (char grid[][MAX_SIZE], int n, char word[], FILE *write_to){
  bool match = false;
  int counter = 0;
  
  // Comparing letters to search word
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
        if(grid[i][j] == word[0]) {
          for(int x = 1; x < (int)strlen(word); x++) {
            if (i + x < MAX_SIZE) {
              if(grid[i + x][j] == word[x]) {
                match = true;
              }
              else {
                match = false;
                break;
              }
            }
            else {
              match = false;
              break;
            }
          }
          if(match == true) {
            fprintf(write_to, "%s %d %d D\n", word, i, j);
            counter++;
          }
        }
      }
    }
  
  return counter;
}


/*
 * This function returns the number of times the given 
 * word string was found in the grid facing up. 
 * Parameter n indicates the actual size of the grid (nxn).
 * The function sends corresponding output to the specified 
 * file pointer, which already points to an open stream. Output lines 
 * must appear in order of the first character's appearance in a 
 * left-to-right scan of each row beginning with row 0.
 */
int find_up   (char grid[][MAX_SIZE], int n, char word[], FILE *write_to){
  int i, len, tempo; 
  char *rev_word; 

  rev_word = (char*)malloc(10);

  strcpy(rev_word, word);
  
  len = (int)strlen(word); 
    
  // Iterating over string to reverse order
  for (i = 0; i < len / 2; i++)  
  {  
    tempo = rev_word[i]; // temporary variable to hold the char
    rev_word[i] = rev_word[len - i - 1];  
    rev_word[len - i - 1] = tempo;  
  }  

  bool match = false;
  int counter = 0;
  
  // Comparing letters to search word
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      if(grid[i][j] == rev_word[0]) {
        for(int x = 1; x < (int)strlen(rev_word); x++) {
          if (i + x < MAX_SIZE) {
            if(grid[i + x][j] == rev_word[x]) {
              match = true;
            }
            else {
              match = false;
              break;
            }
          }
          else {
            match = false;
            break;
          }
        }
        if(match == true) {
          fprintf(write_to, "%s %d %d U\n", word, i, j);
          counter++;
        }
      }
    }
  }
  
  return counter;

}


/*
 * This function reports appearance of the given word in every direction
 * and prints them in the required R,L,D,U order.
 */
int find_all  (char grid[][MAX_SIZE], int n, char word[], FILE *write_to) {
  int right, left, down, up;
  int sum;

  // Calling the functions for every direction in the required order
  right = find_right(grid, n, word, write_to);
  left = find_left(grid, n, word, write_to);
  down = find_down(grid, n, word, write_to);
  up = find_up(grid, n, word, write_to);

  sum = right + left + down + up;

  if (sum == 0) {
    fprintf(write_to, "%s - Not Found\n", word);
  }

  return sum;

} 


/*
 * Reads lhs and rhs character by character until either reaches eof.
 * Returns true if the files can be read and the two files match
 * character by character. Returns false if two files either can't be
 * opened or don't match. The definition of this function is provided 
 * for you.
 */
int file_eq(char lhs_name[], char rhs_name[]) {
  FILE* lhs = fopen(lhs_name, "r");
  FILE* rhs = fopen(rhs_name, "r");

  // don't compare if we can't open the files
  if (lhs == NULL || rhs == NULL) return 0;

  int match = 1;
  // read until both of the files are done or there is a mismatch
  while (!feof(lhs) || !feof(rhs)) {
	if (feof(lhs) ||                      // lhs done first
		feof(rhs) ||                  // rhs done first
		(fgetc(lhs) != fgetc(rhs))) { // chars don't match
	  match = 0;
	  break;
	}
  }
  fclose(lhs);
  fclose(rhs);
  return match;
}
