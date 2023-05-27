/*
 * C implementation of parsort
 * Jiwon Moon, Hajin Jang
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// a helper function for merge / sequential_sort / qsort
// a comparison function is required to use qsort
int compare_i64(const void *l, const void * r) {
  int64_t left = *(const int64_t *)l;
  int64_t right = *(const int64_t *)r;

  if(left < right) {
    return -1;
  } else if (left == right) {
    return 0;
  } else {
    return 1;
  }
}

// Merge the elements in the sorted ranges [begin, mid) and [mid, end),
// copying the result into temparr.
void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  int64_t *endl = arr + mid;
  int64_t *endr = arr + end;
  int64_t *left = arr + begin, *right = arr + mid, *dst = temparr;

  for (;;) {
    int at_end_l = left >= endl;
    int at_end_r = right >= endr;

    if (at_end_l && at_end_r) break;

    if (at_end_l)
      *dst++ = *right++;
    else if (at_end_r)
      *dst++ = *left++;
    else {
      int cmp = compare_i64(left, right);
      if (cmp <= 0)
        *dst++ = *left++;
      else
        *dst++ = *right++;
    }
  }
}

// helper function for merge sort function (sequential sort)
// sequential sort through qsort when the num_elements to sort is small enough
void sequential_sort(int64_t *arr, size_t start, size_t end, int64_t *temp_arr) {
  int num_elements = end - start;
  // copy elements arr[start] to arr[end - 1] into temp_arr[0] to temp_arr[num_elements - 1]
  memcpy(temp_arr, &arr[start], num_elements * sizeof(int64_t));
  qsort(temp_arr, num_elements, sizeof(int64_t), compare_i64);
}

void merge_sort(int64_t *arr, size_t start, size_t end, size_t threshold) {
  int num_elements = end - start;

  if (num_elements <= threshold) { // sort sequentially if num_elements is at or below threshold
    // dynamically allocate a temporary array (to conduct sequential sort on)
    int64_t* temp_arr1 = malloc(num_elements * sizeof(int64_t));
    sequential_sort(arr, start, end, temp_arr1); // sort the temporary array, keep arr unchanged
    memcpy(&arr[start], temp_arr1, num_elements * sizeof(int64_t)); // copy temp_arr1 back into arr
    free(temp_arr1);
  }
  else { // in parallel, sort left & right half of the sequence
    size_t mid = start + num_elements / 2; // compute index of middle element
    pid_t pid_left = fork(); // spawn child process for left half
    pid_t pid_right; // declare pid_right, where we will spawn child process for right half
    
    if (pid_left == -1) { // handle left fork error
      perror("Error: left fork failed");
      exit(pid_left); // exit with -1
    } else if (pid_left == 0) {
      merge_sort(arr, start, mid, threshold); // recursively sort the left half 
      exit(pid_left); // exit with 0
    } else {
      pid_right = fork(); // spawn child process for right half
      if (pid_right == -1) { // handle right fork error
        perror("Error: right fork failed");
        exit(pid_right); // exit with -1
      } else if (pid_right == 0) { 
        merge_sort(arr, mid, end, threshold); // recursively sort the right half
        exit(pid_right); // exit with 0
      }  
    }  

    int wstatus;
    // pause program execution until left child process has completed
    pid_t actual_pid_left = waitpid(pid_left, &wstatus, 0);
    if (actual_pid_left == -1){ // handle waitpid failure
      perror("Error: left waitpid failure");
      exit(-1);
    }
    if (!WIFEXITED(wstatus)) { // handle subprocess failure
      perror("Error: left crashed/was interrupted/did not exit normally");
      exit(-1);
    }
    if (WEXITSTATUS(wstatus) != 0) { // handle non-zero exit code
      perror("Error: left subprocess returned non-zero exit code");
      exit(-1);
    }

    // pause program execution until right child process has completed
    pid_t actual_pid_right = waitpid(pid_right, &wstatus, 0);
    if (actual_pid_right == -1){
      perror("Error: right waitpid failure");
      exit(-1);
    }
    if (!WIFEXITED(wstatus)) { // handle subprocess failure
      perror("Error: right crashed/was interrupted/did not exit normally");
      exit(-1);
    }
    if (WEXITSTATUS(wstatus) != 0) { // handle non-zero exit code
      perror("Error: right subprocess returned non-zero exit code");
      exit(-1);
    }

    int64_t* temp_arr2 = malloc(num_elements * sizeof(int64_t));
    merge(arr, start, mid, end, temp_arr2); // merge fork results into temporary array
    memcpy(&arr[start], &temp_arr2[0], num_elements * sizeof(arr[0])); // copy merged result into array
    free(temp_arr2);
  } 
}

// helper function for main
// checks if an array is sorted in ascending order
// returns 0 if sorted, returns 6 if not sorted
int isSorted(int64_t *arr,int64_t size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 6; // the array is not sorted
        }
    }
    return 0; // the array is sorted
}

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2])) {
    fprintf(stderr, "Error: invalid threshold value");
    return 2;
  }

  int fd = open(filename, O_RDWR);
  if (fd < 0) { // file couldn't be opened: handle error and exit
    fprintf(stderr, "Error: file couldn't be opened");
    return 3;
  }

  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) { // handle fstat error and exit
    fprintf(stderr, "Error: fstat failed");
    return 4;
  }
  size_t file_size_in_bytes = statbuf.st_size;

  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (data == MAP_FAILED) { // handle mmap error and exit
    fprintf(stderr, "Error: mmap failed");
    return 5;
  }

  uint64_t num_elements = file_size_in_bytes / 8; // 8-byte integer values
  merge_sort(data, 0, num_elements, threshold); // check file_size_in_bytes makes sense
  int sort_result = isSorted(data, num_elements); // will be 0 if sort was successful

  munmap(data, file_size_in_bytes); // unmap & check NULL is correct
  close(fd); // close file

  return sort_result; // 0 if sort was successful
}