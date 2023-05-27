/*
 * C implementation of hexdump main function
 * Jiwon Moon, Hajin Jang
 */

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  char data_buf[16];
  char offset_buf[9];
  char byte_buf[3];
  int bytes_read;

  unsigned offset = 0;

  while ((bytes_read = hex_read(data_buf)) > 0) {
    // print offset at the beginning of each line
    hex_format_offset(offset, offset_buf);
    hex_write_string(offset_buf);
    hex_write_string(": ");

    // print hex values for each byte
    for (int i = 0; i < bytes_read; i++) {
        hex_format_byte_as_hex(data_buf[i], byte_buf);
        hex_write_string(byte_buf);
        hex_write_string(" ");
    }

    // fill in spaces to align with standard hexdump output
    for (int i = bytes_read; i < 16; i++) {
        hex_write_string("   ");
    }
    
    hex_write_string(" ");
    // print corresponding printable characters
    for (int i = 0; i < bytes_read; i++) {
      char printable_char[2];
      printable_char[0] = hex_to_printable(data_buf[i]);
      printable_char[1] = '\0';
      hex_write_string(printable_char);
    }


    hex_write_string("\n");
    offset += bytes_read;
}

return 0;

}
