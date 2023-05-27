/*
 * Header file that contains the declarations for the
 * helper functions for the UInt256 data structure.
 * Hajin Jang
 */

#ifndef UINT256_H
#define UINT256_H

#include <stdint.h>

// Data type representing a 256-bit unsigned integer, represented
// as an array of 4 uint64_t values. It is expected that the value
// at index 0 is the least significant, and the value at index 3
// is the most significant.
//
// Don't make any changes to the representation of this data type.
// (I.e., don't add fields, remove fields, change names of fields, etc.)
typedef struct {
  uint64_t data[4];
} UInt256;


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
UInt256 uint256_create_from_u64(uint64_t val);


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
UInt256 uint256_create(const uint64_t data[4]);


/*
 * Create a UInt256 value from a string of hexadecimal digits.
 *
 * Parameters:
 *   hex - a pointer to a char array that contains hexadecimal digits
 *
 * Returns:
 *   a UInt256 object created from the hexadecimal digits in hex
 */
UInt256 uint256_create_from_hex(const char *hex);


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
char *uint256_format_as_hex(UInt256 val);


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
uint64_t uint256_get_bits(UInt256 val, unsigned index);


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
UInt256 uint256_add(UInt256 left, UInt256 right);


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
UInt256 uint256_sub(UInt256 left, UInt256 right);


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
int uint256_bit_is_set(UInt256 val, unsigned index);


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
UInt256 uint256_leftshift(UInt256 val, unsigned shift);


/*
 * Compute the product of two UInt256 values.
 *
 * Parameters:
 *   left - a UInt256 object that is the first operand
 *   right - a UInt256 object that is the second operand
 *
 * Returns:
 *   the result of the multiplication as a UInt256 value
 */UInt256 uint256_mul(UInt256 left, UInt256 right);

#endif // UINT256_H
