/*
 * C implementation of hexdump functions
 * Jiwon Moon, Hajin Jang
 */

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

/*
 * Read up to 16 bytes from standard input into data_buf.
 *
 * Parameters:
 *   data_buf - a char array to store the hex data read
 *
 * Returns:
 *   unsigned variable that contains number of characters read
 *   a UInt256 object created from val (a single uint64_t value)
 *
 */
unsigned hex_read(char data_buf[]) {
    ssize_t bytes_read;
    size_t buf_size = 0;
    while (data_buf[buf_size] != '\0') {
        buf_size++;
    }

    bytes_read = read(STDIN_FILENO, data_buf, 16);

    return (unsigned)bytes_read;
    // if bytes_read == -1, error; if bytes_read == 0, end of input reached
}

/*
 * Write given nul-terminated string to standard output.
 *
 * Parameters:
 *   s - a char array that stores the data to 'write'(print)
 * 
 */
 void hex_write_string(const char s[]) {
    size_t s_size = 0;
    while (s[s_size] != '\0') {
        s_size++;
    }
    
    write(STDOUT_FILENO, s, s_size);
    // if bytes_written == -1, error
}

/*
 * Format an unsigned value as an offset string consisting of exactly 8
 * hex digits.  The formatted offset is stored in sbuf, which must
 * have enough room for a string of length 8.
 *
 * Parameters:
 *   offset - an unsigned variable that contains the hexadecimal memory address
 *   sbuf - a char array that will contain the formatted offset string
 * 
 */
void hex_format_offset(unsigned offset, char sbuf[]) {
    for (int i = 7; i >= 0; i--) {
        sbuf[i] = "0123456789abcdef"[offset & 0xf];
        offset >>= 4;
    }
    sbuf[8] = '\0';
}

/*
 * Format a byte value (in the range 0-255) as string consisting
 * of two hex digits.  The string is stored in sbuf.
 *
 * Parameters:
 *   byteval - the byte value to be formatted
 *   sbuf - a char array that will contain the formatted byte value
 *
 */
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
    sbuf[0] = "0123456789abcdef"[byteval >> 4];
    sbuf[1] = "0123456789abcdef"[byteval & 0xf];
    sbuf[2] = '\0';
}

/*
 * Convert a byte value (in the range 0-255) to a printable character
 * value.  If byteval is already a printable character, it is returned
 * unmodified.  If byteval is not a printable character, then the
 * ASCII code for '.' should be returned.
 *
 * Parameters:
 *   byteval - the byte value to be converted
 *
 * Returns:
 *   a char variable that contains the byte value converted
 *   to a printable character
 *
 */
char hex_to_printable(unsigned char byteval) {
    if (byteval >= 32 && byteval <= 126) {
        // printable character
        return byteval;
    } else {
        // non-printable character
        return '.';
    }
}
