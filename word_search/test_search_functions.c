// test_search_functions.c
// <STUDENT: ADD YOUR INFO HERE: name, JHED, etc.>



#include <stdio.h>
#include <assert.h>
#include "search_functions.h"


/* 
 * Declarations for tester functions whose definitions appear below.
 * (You will need to fill in the function definition details, at the
 * end of this file, and add comments to each one.) 
 * Additionally, for each helper function you elect to add to the 
 * provided search_functions.h, you will need to supply a corresponding
 * tester function in this file.  Add a declaration for it here, its
 * definition below, and a call to it where indicated in main.
 */
void test_file_eq();      // This one is already fully defined below.
void test_populate_grid();
void test_find_right();
void test_find_left();
void test_find_down();
void test_find_up();
void test_find_all();


/*
 * Main method which calls all test functions.
 */
int main() {
  printf("Testing file_eq...\n");
  test_file_eq();
  printf("Passed file_eq test.\n\n");

  printf("Running search_functions tests...\n");
  test_populate_grid();
  test_find_right();
  test_find_left();
  test_find_down();
  test_find_up();
  test_find_all();

  /* You may add calls to additional test functions here. */

  printf("Passed search_functions tests!!!\n");
}



/*
 * Test file_eq on same file, files with same contents, files with
 * different contents and a file that doesn't exist.
 * Relies on files test1.txt, test2.txt, test3.txt being present.
 */
void test_file_eq() {
  FILE* fptr = fopen("test1.txt", "w");
  fprintf(fptr, "this\nis\na test\n");
  fclose(fptr);

  fptr = fopen("test2.txt", "w");
  fprintf(fptr, "this\nis\na different test\n");
  fclose(fptr);

  fptr = fopen("test3.txt", "w");
  fprintf(fptr, "this\nis\na test\n");
  fclose(fptr);

  assert( file_eq("test1.txt", "test1.txt"));
  assert( file_eq("test2.txt", "test2.txt"));
  assert(!file_eq("test2.txt", "test1.txt"));
  assert(!file_eq("test1.txt", "test2.txt"));
  assert( file_eq("test3.txt", "test3.txt"));
  assert( file_eq("test1.txt", "test3.txt"));
  assert( file_eq("test3.txt", "test1.txt"));
  assert(!file_eq("test2.txt", "test3.txt"));
  assert(!file_eq("test3.txt", "test2.txt"));
  assert(!file_eq("", ""));  // can't open file
}



void test_populate_grid(){
  char grid1[4][4] = {{'p','i','t','k'},{'o','l','p','e'},{'p','k','e','y'},{'t','o','p','e'}};
  char grid2[MAX_SIZE][MAX_SIZE];
  populate_grid(grid2, "test4.txt");

  // Check if hard-coded grid equals the populated grid filled by the program
  for(int x = 0; x < 4; x++) {
    for(int y = 0; y < 4; y++) {
      assert(grid1[x][y] == grid2[x][y]);
    }
  }

}


void test_find_right(){
  FILE *test5 = fopen("test5.txt", "w");
  char grid_right[4][10] = {{'p','i','t','k'},{'o','l','p','e'},{'p','k','e','y'},{'t','o','p','e'}};
  char word_right[] = "pot"; //word to search
  int count_right = find_right(grid_right, 4, word_right, test5);
  
  assert(count_r == 1);

  fclose(test5);

  assert(file_eq("test5.txt", "pot.txt"))

}


void test_find_left(){
  FILE *test6 = fopen("test6.txt", "w");
  char grid_right[4][10] = {{'p','i','t','k'},{'o','l','p','e'},{'p','k','e','y'},{'t','o','p','e'}};
  char word_right[] = "tip"; //word to search
  int count_right = find_left(grid_right, 4, word_right, test6);
  
  assert(count_r == 1);

  fclose(test5);
  
  assert(file_eq("test6.txt", "tip.txt"));
}


void test_find_down(){

  FILE *test7 = fopen("test5.txt", "w");
  char grid_right[4][10] = {{'p','i','t','k'},{'o','l','p','e'},{'p','k','e','y'},{'t','o','p','e'}};
  char word_right[] = "tip"; //word to search
  int count_right = find_left(grid_right, 4, word_right, test7);
  
  assert(count_r == 1);

  fclose(test7);
  
  assert(file_eq("test7.txt", "pop.txt"));

}


void test_find_up(){

  assert(1);  //replace this stub!

}


void test_find_all(){

  assert(1);  //replace this stub!

}
