/*
 * Helper functions for the UInt256 data structure
 * Hajin Jang
 */


#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "uint256.h"


/*
 * Create a UInt256 value from a single uint64_t value.
 * Only the least-significant 64 bits are initialized directly,
 * all other bits are set to 0.
 *
 * Parameters:
 *   val - unsigned integer of size 64 bits
 *
 * Returns:
 *   a UInt256 object created from val (a single uint64_t value)
 */
UInt256 uint256_create_from_u64(uint64_t val) {
  UInt256 result = (UInt256) {{val, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0}};
  return result;
}


/*
 * Create a UInt256 value from an array of 4 uint64_t values.
 * The element at index 0 is the least significant, and the element
 * at index 3 is the most significant.
 *
 * Parameters:
 *   data - a length 4 array of uint64_t type integers
 *
 * Returns:
 *   a UInt256 object created from data (multiple uint64_t values)
 */
UInt256 uint256_create(const uint64_t data[4]) {
  UInt256 result;
  for (int i = 0; i < 4; i++) {
    result.data[i] = data[i]; // copy contents in data to result.data
  }
  return result;
}


/*
 * Create a UInt256 value from a string of hexadecimal digits.
 *
 * Parameters:
 *   hex - a pointer to a char array that contains hexadecimal digits
 *
 * Returns:
 *   a UInt256 object created from the hexadecimal digits in hex
 */
UInt256 uint256_create_from_hex(const char *hex) {
  UInt256 result = {0};
  int start = strlen(hex);
  int end = strlen(hex) - 64; // only use rightmost 64 digits
  if (strlen(hex) <= 64) {
    end = 0;
  }
  size_t convert = 0; // keep track of the number of hex digits processed
  int data_index = 0; // variable used to keep track of result.data index (0-3)
  while ((strlen(hex) > convert) && (end < start)) {
    int len_left = 16;
    char temp[17]; // char array to store 16 specified hex digits
    if (end > (start - 16)) { // if < 16 digits left, only process remaining digits
      len_left = strlen(hex) - convert;
    }
    if (0 > len_left) { // if len_left negative, set to 0
      len_left = 0;
    }
    memcpy(temp, hex - len_left + start, len_left); // copy 16 (or less) hex digits into temp
    temp[len_left] = '\0'; // terminate temp using null character
    result.data[data_index++] = strtoul(temp, NULL, 16); // store converted value
    start = start - len_left;
    convert += len_left;
  }
  return result;
}


/*
 * Return a dynamically-allocated string of hex digits representing
 * the given UInt256 value.
 *
 * Parameters:
 *   val - a UInt256 object with data to be converted into hex digits
 *
 * Returns:
 *   a pointer to a char array that contains hexadecimal digits
 */
char *uint256_format_as_hex(UInt256 val) {
  char *hex = malloc (sizeof(char) * 64 + 1);
  hex[0] = '\0';
  char buffer[17];
  for (int i = 3; i >= 0; i--) {
    uint64_t digit = val.data[i];
    // case 1: hex is not empty
    if (strlen(hex) != 0) {
      sprintf(buffer, "%016lx", digit); // concat with leading 0's
      strcat(hex, buffer);
    }
    // case 2: hex is empty & current value != 0
    else if (strlen(hex) == 0 && digit != 0) {
      sprintf(buffer, "%lx", digit); // concat without leading 0's
      strcat(hex, buffer);
    }
  }
  // case 3: hex is 0
  if (strlen(hex) == 0) {
    strcat(hex, "0");
  }
  return hex; // assume hex will be freed where it is being called
}


/*
 * Get 64 bits of data from a UInt256 value.
 * Index 0 is the least significant 64 bits, index 3 is the most
 * significant 64 bits.
 *
 * Parameters:
 *   val - a UInt256 object
 *   index - non-negative integer
 *
 * Returns:
 *   64 bits of data from val.data at the specified index
 */
uint64_t uint256_get_bits(UInt256 val, unsigned index) {
  assert(index < 4); // check if index is out of bounds
  uint64_t bits = val.data[index];
  return bits;
}


/*
 * Compute the sum of two UInt256 values.
 *
 * Parameters:
 *   left - a UInt256 object 
 *   right - a UInt256 object
 *
 * Returns:
 *   a UInt256 object that is the sum of left and right
 */
UInt256 uint256_add(UInt256 left, UInt256 right) {
  UInt256 sum;
  uint64_t carry = 0;
  
  for (int i = 0; i < 4; i++) {
    sum.data[i] = left.data[i] + right.data[i] + carry;
    carry = (sum.data[i] < left.data[i]) ? 1 : 0; // check for overflow
  }

  return sum;
}


/*
 * Compute the difference of two UInt256 values.
 *
 * Parameters:
 *   left - a UInt256 object to subtract right from
 *   right - a UInt256 object to be negated and added to left
 *
 * Returns:
 *   a UInt256 object that is the result of subtracting right from left
 */
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  UInt256 result = uint256_create_from_u64(0UL);

  for (int i = 0; i < 4; i++) {
    // add the negated right + 1 to left, while accounting for overflow
    uint64_t sub_val = left.data[i] + (~right.data[i] + 1) + result.data[i];

    // check for negative overflows
    if ((i < 3) && (sub_val > left.data[i])) {
      result.data[i + 1] -= 1;
    }
    result.data[i] = sub_val;
  }
  return result;
} 


/*
 * Checks whether a particular bit is set to 1.
 *
 * Parameters:
 *   val - a UInt256 object in which the bit to check is stored
 *   index - unsigned data that specifies which bit to check
 *
 * Returns:
 *   1 if the bit is set, 0 otherwise
 */
int uint256_bit_is_set(UInt256 val, unsigned index) {
  unsigned word = index / 64;
  unsigned bit = index % 64;
  return (val.data[word] & (1UL << bit)) != 0; // access and compare bit at index
}


/*
 * Left-shifts a value by a specified number of positions.
 *
 * Parameters:
 *   val - a UInt256 object in which the bit to shift is stored
 *   shift - unsigned data that specifies how many positions to shift
 *
 * Returns:
 *   a UInt256 object that is the result of left shift operation
 */
UInt256 uint256_leftshift(UInt256 val, unsigned shift){
  UInt256 result;
  // ensure shift is less than 256
  assert(shift < 256);
  // return val if shift is 0
  if (shift == 0) {
    return val;
  }
  uint64_t carry = 0;
  unsigned digit_shift = shift / 64; // num between 1-3
  unsigned bit_shift = shift % 64; // num bits to shift (0-63)
  for (unsigned i = 0; i < 4 - digit_shift; i++) {
    uint64_t cur = val.data[i];
    uint64_t shifted = cur << bit_shift;
    result.data[i + digit_shift] = shifted + carry;
    carry = ((1UL << bit_shift) - 1UL) & (cur >> (64UL - bit_shift)); // shift leftmost num of bits
  }
  for (unsigned j = 0; j < digit_shift; j++) { // Clear shifted out values in result
    result.data[j] = 0UL;
  }
  return result;
}


/*
 * Compute the product of two UInt256 values.
 *
 * Parameters:
 *   left - a UInt256 object that is the first operand
 *   right - a UInt256 object that is the second operand
 *
 * Returns:
 *   the result of the multiplication as a UInt256 value
 */
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  UInt256 product = uint256_create_from_u64(0UL);
  for (unsigned i = 0; i < 256; i++) {
    if (uint256_bit_is_set(left, i)) {
      product = uint256_add(product, uint256_leftshift(right, i));
    }
  }
  return product;
}

