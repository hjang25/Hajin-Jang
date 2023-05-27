//project.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_io.h"
#include "image_manip.h"

// Return (exit) codes
#define RC_SUCCESS            0
#define RC_MISSING_FILENAME   1
#define RC_OPEN_FAILED        2
#define RC_INVALID_PPM        3
#define RC_INVALID_OPERATION  4
#define RC_INVALID_OP_ARGS    5
#define RC_OP_ARGS_RANGE_ERR  6
#define RC_WRITE_FAILED       7
#define RC_UNSPECIFIED_ERR    8

void print_usage();

int main(int argc, char* argv[]) { 
  // argv[0] : name of executable
  // argv[1] : name of input file
  // argv[2] : name of output file
  // argv[3] : operation to be executed
  // argv[4] and on : parameters for operations (if valid)

  // less than 2 command line args means that input or output filename
  // wasn't specified since argv[1] is input file name and
  // argv[2] is output file name
  if (argc < 3) {
    fprintf(stderr, "Missing input/output filenames\n");
    print_usage();
    return RC_MISSING_FILENAME;
  }

  // open file to read input file
  FILE *to_read = NULL;
  to_read = fopen(argv[1], "rb");

  // catch input file I/O error
  if (to_read == NULL) {
    fprintf(stderr, "Input file I/O error\n");
    print_usage();
    return RC_OPEN_FAILED;
  }

  // call read_ppm function
  Image *im_read = read_ppm(to_read);

  // close file as soon as done using
  fclose(to_read);

  // catch error in reading ppm
  if (im_read == NULL) {
    fprintf(stderr, "The Input file cannot be read as a PPM file\n");
    print_usage();
    return RC_INVALID_PPM;
  }

  // open file to write output
  FILE *to_write = NULL;
  to_write = fopen(argv[2], "wb");

  // catch output file error
  if (to_write == NULL) {
    fprintf(stderr, "Output file I/O error\n");
    print_usage();
    fclose(to_write); // close output file
    return RC_WRITE_FAILED;
  }

  // swap operation
  if (strcmp(argv[3], "swap") == 0) {
    // catch error when more arguments are given than the required number
    if (argc > 4) {
      fprintf(stderr, "Incorrect number of arguments for the specified operation\n");
      print_usage();
      // free dynamically allocated memory & close file(s)
      free_image(&im_read);
      fclose(to_write);
      return RC_INVALID_OP_ARGS;
    }

    // call swap
    swap(im_read);

    // call write ppm function
    int num_pix = write_ppm(to_write, im_read);

    // catch output I/O errors
    if (num_pix == -1) {
      fprintf(stderr, "Output file I/O error\n");
      print_usage();
      free_image(&im_read);
      fclose(to_write);
      return RC_WRITE_FAILED;
    }

    free_image(&im_read);
  }
  // invert operation
  else if (strcmp(argv[3], "invert") == 0) {
    // catch error when more arguments are given than the required number
    if (argc > 4) {
      fprintf(stderr, "Incorrect number of arguments for the specified operation\n");
      print_usage();
      free_image(&im_read);
      fclose(to_write);
      return RC_INVALID_OP_ARGS;
    }

    // call invert
    invert(im_read);

    // call write ppm function
    int num_pix = write_ppm(to_write, im_read);

    // catch output I/O errors
    if (num_pix == -1) {
      fprintf(stderr, "Output file I/O error\n");
      print_usage();
      free_image(&im_read);
      fclose(to_write);
      return RC_WRITE_FAILED;
    }
     
    free_image(&im_read);
  }
  // zoom out operation
  else if (strcmp(argv[3], "zoom-out") == 0) {
    // catch error when more arguments are given than the required number
    if (argc > 4) {
      fprintf(stderr, "Incorrect number of arguments for the specified operation\n");
      print_usage();
      free_image(&im_read);
      fclose(to_write);
      return RC_INVALID_OP_ARGS;
    }
    // create new image of 1/4 the size of given image
    Image *new_im = make_image((im_read->rows) / 2, (im_read->cols) / 2);

    // call zoom_out
    zoom_out(im_read, new_im);

    // call write function
    int num_pix = write_ppm(to_write, new_im);

    // free dynamically allocated memory
    free_image(&new_im);

    // catch output I/O errors
    if (num_pix == -1) {
      fprintf(stderr, "Output file I/O error\n");
      print_usage();
      free_image(&im_read);
      fclose(to_write);
      return RC_WRITE_FAILED;
    }
  }
  // rotate right operation
  else if (strcmp(argv[3], "rotate-right") == 0) {
    // catch error when more arguments are given than the required number
    if (argc > 4) {
      fprintf(stderr, "Incorrect number of arguments for the specified operation\n");
      print_usage();
      free_image(&im_read);
      fclose(to_write);
      return RC_INVALID_OP_ARGS;
    }

    // create new image with reversed dimensions
    Image *new_im = make_image(im_read->cols, im_read->rows);

    // call rotate_right
    rotate_right(im_read, new_im);

    // call write ppm function
    int num_pix = write_ppm(to_write, new_im);

    // free dynamically allocated memory
    free_image(&new_im);

    // catch output I/O errors
    if (num_pix == -1) {
      fprintf(stderr, "Output file I/O error\n");
      print_usage();
      free_image(&im_read);
      fclose(to_write);
      return RC_WRITE_FAILED;
    }
  }
  // swirl operation
  else if (strcmp(argv[3], "swirl") == 0) {
    // catch error when not all required arguments are provided
    if (argc < 7) {
      fprintf(stderr, "Incorrect number of arguments for the specified operation\n");
      print_usage();
      fclose(to_write);
      free_image(&im_read);
      return RC_INVALID_OP_ARGS;
    }
    // catch error when more arguments are given than the required number
    if (argc > 7) {
      fprintf(stderr, "Incorrect number of arguments for the specified operation\n");
      print_usage();
      fclose(to_write);
      free_image(&im_read);
      return RC_INVALID_OP_ARGS;
    }

    double cx = atof(argv[4]);
    double cy = atof(argv[5]);
    int s = atoi(argv[6]);

    // catch invalid arguments
    if (cx < -1 || cx > (im_read->cols - 1)) {
      fprintf(stderr, "Invalid arguments for the specified operation\n");
      print_usage();
      fclose(to_write);
      free_image(&im_read);
      return RC_OP_ARGS_RANGE_ERR;
    }
    if (cy < -1 || cy > (im_read->rows - 1)) {
      fprintf(stderr, "Invalid arguments for the specified operation\n");
      print_usage();
      fclose(to_write);
      free_image(&im_read);
      return RC_OP_ARGS_RANGE_ERR;
    }
    if (s < 0) {
      fprintf(stderr, "Invalid arguments for the specified operation\n");
      print_usage();
      fclose(to_write);
      free_image(&im_read);
      return RC_OP_ARGS_RANGE_ERR;
    }

    // create new image to save swirled image
    Image *new_im = make_image(im_read->rows, im_read->cols);

    // call swirl
    swirl(im_read, new_im, cx, cy, s);

    // call write ppm function
    int num_pix = write_ppm(to_write, new_im);

    // free dynamically allocated memory
    free_image(&new_im);

    // catch output I/O errors
    if (num_pix == -1) {
      fprintf(stderr, "Output file I/O error\n");
      print_usage();
      free_image(&im_read);
      fclose(to_write);
      return RC_WRITE_FAILED;
    }
  }
  // edge detection operation
  else if (strcmp(argv[3], "edge-detection") == 0) {
    // catch error when not all required arguments are provided
    if (argv[4] == NULL) {
      fprintf(stderr, "Incorrect number of arguments for the specified operation\n");
      print_usage();
      fclose(to_write);
      free_image(&im_read);
      return RC_INVALID_OP_ARGS;
    }

    // catch error when more arguments are given than the required number
    if (argc > 5) {
      fprintf(stderr, "Incorrect number of arguments for the specified operation\n");
      print_usage();
      fclose(to_write);
      free_image(&im_read);
      return RC_INVALID_OP_ARGS;
    }

    // call edges
    double t = atof(argv[4]);
    Image *returned = edges(im_read, t);

    // call write ppm function
    int num_pix = write_ppm(to_write, returned);

    // free dynamically allocated memory
    free_image(&returned);

    // catch output I/O errors
    if (num_pix == -1) {
      fprintf(stderr, "Output file I/O error\n");
      print_usage();
      // free dynamically allocated memory & close files
      free_image(&im_read);
      free_image(&returned);
      fclose(to_write);
      return RC_WRITE_FAILED;
    }
  }
  else { // return 4 when incorrect operation name is provided
    fprintf(stderr, "Unsupported image processing operation\n");
    print_usage();
    free_image(&im_read);
    fclose(to_write);
    return RC_INVALID_OPERATION;
  }
  return RC_SUCCESS; // return 0 when no errors caught & operation succeeded
}

void print_usage() {
  printf("USAGE: ./project <input-image> <output-image> <command-name> <command-args>\n");
  printf("SUPPORTED COMMANDS:\n");
  printf("   swap\n");
  printf("   invert\n");
  printf("   zoom-out\n");
  printf("   rotate-right\n");
  printf("   swirl <cx> <cy> <strength>\n");
  printf("   edge-detection <threshold>\n");
}
