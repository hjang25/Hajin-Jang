#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "image_manip.h"
#include "ppm_io.h"

/* HELPER for grayscale:
 * convert a RGB pixel to a single grayscale intensity;
 * uses NTSC standard conversion
 */
unsigned char pixel_to_gray(const Pixel *p) {
  return (unsigned char)( (0.3 * (double)p->r) +
                          (0.59 * (double)p->g) +
                          (0.11 * (double)p->b) );
}

/* ______grayscale______
 * convert an image to grayscale (NOTE: pixels are still
 * RGB, but the three values will be equal)
 */
void grayscale(Image *im) {
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - grayscale given a bad image pointer\n");
    return;
  }

  for (int r=0; r<im->rows; r++){
    for (int c=0; c<im->cols; c++){
      // find grayscale intensity
      unsigned char grayLevel = pixel_to_gray(&(im->data[(r*im->cols)+c]));

      // set all chanels to it
      im->data[(r*im->cols)+c].r = grayLevel;
      im->data[(r*im->cols)+c].g = grayLevel;
      im->data[(r*im->cols)+c].b = grayLevel;

    }
  }
}



/* ______swap______
 * swap color channels of an image
 */
void swap(Image *im) {
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - swap given a bad image pointer\n");
    return;
  }

  // iterate over pixels stored in im->data to 'swap' rgb values
  for (int r = 0; r < (im->rows); r++) {
    for (int c = 0; c < (im->cols); c++) {
      // temporary variable stores value of red
      unsigned char temp = im->data[(r*im->cols)+c].r; // temporary variable to store red value

      // red gets value of green
      im->data[(r*im->cols)+c].r = im->data[(r*im->cols)+c].g;

      // green gets value of blue
      im->data[(r*im->cols)+c].g = im->data[(r*im->cols)+c].b;

      // blue gets value of red
      im->data[(r*im->cols)+c].b = temp;
    }
  }
}



/* ______invert______
 * invert the intensity of each channel
 */
 void invert(Image *im) {
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - invert given a bad image pointer\n");
    return;
  }

  // iterate over pixels stored in im->data to 'invert' rgb values
  for (int r = 0; r < (im->rows); r++) {
    for (int c = 0; c < (im->cols); c++) {
      // red = 255 - red
      im->data[(r*im->cols)+c].r = 255 - im->data[(r*im->cols)+c].r;

      // green = 255 - green
      im->data[(r*im->cols)+c].g = 255 - im->data[(r*im->cols)+c].g;

      // blue = 255 - blue
      im->data[(r*im->cols)+c].b = 255 - im->data[(r*im->cols)+c].b;
    }
  }
 }



/* ______zoom_out______
 * "zoom out" an image, by taking a 2x2 square of pixels and averaging
 * each of the three color channels to make a single pixel. If an odd
 * number of rows in original image, we lose info about the bottom row.
 * If an odd number of columns in original image, we lose info about the
 * rightmost column.
 */
 void zoom_out(Image *im, Image *new_im) {
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - zoom_out given a bad image pointer\n");
    return;
  }
  if (!new_im || !new_im->data) {
        fprintf(stderr, "Error:image_manip - zoom_out given a bad image pointer\n");
        return;
  }

  // halve num of rows and cols
  int cols = (im->cols)/2; // if im->cols is odd, then int casting will lose last row
  int rows = (im->rows)/2;

  int index = 0; // index of zoomed out image
  int start = 0; // index of first pixel in 2x2 square

  // iterate to compute rgb values for zoomed-out image
  int r = 0, c = 0; // counters for old image
  for (int count_r = 0; count_r < rows; count_r++) {
    for (int count_c = 0; count_c < cols; count_c++) {
      start = (r * im->cols) + (c); // assign index of 1st pixel in 2x2 square

      // first pixel rgb values (i.e., upper left pixel of 2x2 square)
      int red_0 = im->data[start].r;
      int green_0 = im->data[start].g;
      int blue_0 = im->data[start].b;
      
      // second pixel rgb values (i.e., upper right pixel)
      int red_1 = im->data[start + 1].r;
      int green_1 = im->data[start + 1].g;
      int blue_1 = im->data[start + 1].b;
      
      // third pixel rgb values (i.e., bottom left pixel)
      int red_2 = im->data[(r + 1) * im->cols + c].r;
      int green_2 = im->data[(r + 1) * im->cols + c].g;
      int blue_2 = im->data[(r + 1) * im->cols + c].b;
      
      // forth pixel rgb values (i.e., bottom right pixel)
      int red_3 = im->data[(r + 1) * im->cols + c + 1].r;
      int green_3 = im->data[(r + 1) * im->cols + c + 1].g;
      int blue_3 = im->data[(r + 1) * im->cols + c + 1].b;

      index = count_r * cols + count_c; // index for zoomed out image to assign rgb values to

      // compute average rgb values of 4 pixels in 2x2 box
      new_im->data[index].r = (red_0 + red_1 + red_2 + red_3) / 4;
      new_im->data[index].g = (green_0 + green_1 + green_2 + green_3) / 4;
      new_im->data[index].b = (blue_0 + blue_1 + blue_2 + blue_3) / 4;

      c += 2; // move to the next 2x2 square on the same row
    }
    r += 2; // move to the next 2x2 square once end of row is reached
    c = 0; // start from leftmost 2x2 square in next row
  }
  free_image(&im);
 }



/* _______rotate-right________
 * rotate the input image clockwise 90 degrees
 */
 void rotate_right(Image *im, Image *new_im) {
  // catch possible errors
  if (!im || !im->data) {
      fprintf(stderr, "Error:image_manip - rotate_right given a bad image pointer\n");
      return;
  }
  if (!new_im || !new_im->data) {
      fprintf(stderr, "Error:image_manip - rotate_right given a bad image pointer\n");
      return;
  }

  int index = 0; // variable to store index of each pixel in im->data

  // iterate over im->data to reassign pixels to new locations
  for(int r = 0; r < (new_im->rows); r++) {
    for (int c = 0; c < (new_im->cols); c++) {
      index = im->cols * (im->rows - 1 - c) + r; // compute index from 2D to 1D
      new_im->data[r * new_im->cols + c] = im->data[index];
    }
  }
  free_image(&im);
 }



/* ________Swirl effect_________
 * Create a whirlpool effect!
 */
  void swirl(Image *im, Image *new_im, double cx, double cy, int s) {
    if (!im || !im->data) {
        fprintf(stderr, "Error:image_manip - swirl given a bad image pointer\n");
        return;
    }
    if (!new_im || !new_im->data) {
        fprintf(stderr, "Error:image_manip - swirl given a bad image pointer\n");
        return;
    }

    // set cx and/or cy as image center
    if (cx == -1 ) {
        cx = im->cols / 2.;
    }
    if (cy == -1) {
      cy = im->rows / 2.;
    }

    double alpha = 0; // variable to store the computed alpha value
    double sx = 0, sy = 0; // variables to store sx and sy coordinates
    Point Is; // Point structure variable to store sx and sy

    // iterate over im->data to reassign Pixels to calculated locations
    for (int y = 0; y < (im->rows); y++) {
      for (int x = 0; x < (im->cols); x++) {

        double new_x = x - cx;
        double new_y = y - cy;

        // compute alpha
        alpha = sqrt(pow(new_x, 2) + pow(new_y, 2)) / (double)s;

        // compute sx and sy
        sx = new_x * cos(alpha) - new_y * sin(alpha) + cx;
        sy = new_x * sin(alpha) + new_y * cos(alpha) + cy;

        // cast double values (sx, sy) into integers
        Is.x = sx;
        Is.y = sy;

        // make pixel black if sx and/or sy out of bound
        if (0 <= Is.x && Is.x < im->cols) {
          if (0 <= Is.y && Is.y < im->rows) {
            // assign data from calculated coordinate (sx, sy) of original image to new image
            new_im->data[y * im->cols + x] = im->data[Is.y * im->cols + Is.x];
          }
          else {
            new_im->data[y * im->cols + x].r = 0;
            new_im->data[y * im->cols + x].g = 0;
            new_im->data[y * im->cols + x].b = 0;
          }
        }
        else {
          new_im->data[y * im->cols + x].r = 0;
          new_im->data[y * im->cols + x].g = 0;
          new_im->data[y * im->cols + x].b = 0;
        }
    }
  }
  free_image(&im);
 }

/* _______edges________
 * apply edge detection as a grayscale conversion
 * followed by an intensity gradient computation and
 * thresholding
 */
Image * edges(Image *im, double t) {
  if (!im || !im->data) {
        fprintf(stderr, "Error:image_manip - edge detection given a bad image pointer\n");
        return NULL;
  }
  grayscale(im);

    Image *new_im = make_copy(im);

    double ix, iy; // intensity gradient in horizontal and vertical directions
    double mag; // magnitue of gradient

    // start from 1 and end at im->rows/cols - 1 to skip boundary pixels
    for (int y = 1; y < (im->rows) - 1; y++) {
      for (int x = 1; x < (im->cols) - 1; x++) {
        // index of each pixel in im->data
        int index = (y * im->cols) + x;

        // compute intensity gradient for each interior point
        // with index == (y * im->cols) + x. Since r, g, b values
        // are same for grayscale image, will use value of r
        ix = (im->data[index + 1].r - im->data[index - 1].r) / 2.;
        iy = (im->data[index + im->cols].r - im->data[index - im->cols].r) / 2.;

        // compute magnitude of intensity gradient(s)
        mag = sqrt(pow(ix, 2) + pow(iy, 2));

        // compare mag and threshold value t
        if (mag > t) {
          // set to black
          new_im->data[index].r = 0;
          new_im->data[index].g = 0;
          new_im->data[index].b = 0;
        }
        else {
          // set to white
          new_im->data[index].r = 255;
          new_im->data[index].g = 255;
          new_im->data[index].b = 255;
        }
    }
  }
  free_image(&im);
  return new_im;
}




