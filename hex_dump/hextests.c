/*
 * Unit tests for hex functions
 * These tests should work for both your C implementations and your
 * assembly language implementations
 * Jiwon Moon, Hajin Jang
 */

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
  char test_data_2[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  strcpy(objs->test_data_2, "!\"#$&\'()\n"); // special symbols: !"#$&'()
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);
// void testHexRead(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(0x00000001u, buf);
  ASSERT(0 == strcmp(buf, "00000001"));
  hex_format_offset(0xabcd1234u, buf);
  ASSERT(0 == strcmp(buf, "abcd1234")); 
  hex_format_offset(0x00000048u, buf);
  ASSERT(0 == strcmp(buf, "00000048")); 
  hex_format_offset(0x10030448u, buf);
  ASSERT(0 == strcmp(buf, "10030448")); 
  // random hex: 1daca13b 545bddea ee44bc52 ad5b516c 4726db04
  hex_format_offset(0x1daca13bu, buf);
  ASSERT(0 == strcmp(buf, "1daca13b")); 
  hex_format_offset(0x545bddeau, buf);
  ASSERT(0 == strcmp(buf, "545bddea")); 
  hex_format_offset(0xee44bc52u, buf);
  ASSERT(0 == strcmp(buf, "ee44bc52")); 
  hex_format_offset(0xad5b516cu, buf);
  ASSERT(0 == strcmp(buf, "ad5b516c")); 
  hex_format_offset(0x4726db04u, buf);
  ASSERT(0 == strcmp(buf, "4726db04")); 
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  // Hello, world! ==> 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));
  hex_format_byte_as_hex(objs->test_data_1[1], buf);
  ASSERT(0 == strcmp(buf, "65"));
  hex_format_byte_as_hex(objs->test_data_1[2], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[3], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[4], buf);
  ASSERT(0 == strcmp(buf, "6f"));
  hex_format_byte_as_hex(objs->test_data_1[5], buf);
  ASSERT(0 == strcmp(buf, "2c"));
  hex_format_byte_as_hex(objs->test_data_1[6], buf);
  ASSERT(0 == strcmp(buf, "20"));
  hex_format_byte_as_hex(objs->test_data_1[7], buf);
  ASSERT(0 == strcmp(buf, "77"));
  hex_format_byte_as_hex(objs->test_data_1[8], buf);
  ASSERT(0 == strcmp(buf, "6f"));
  hex_format_byte_as_hex(objs->test_data_1[9], buf);
  ASSERT(0 == strcmp(buf, "72"));
  hex_format_byte_as_hex(objs->test_data_1[10], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[11], buf);
  ASSERT(0 == strcmp(buf, "64"));
  hex_format_byte_as_hex(objs->test_data_1[12], buf);
  ASSERT(0 == strcmp(buf, "21"));

  // speical symbols: !"#$&'() ==> 21 22 23 24 26 27 28 29
  hex_format_byte_as_hex(objs->test_data_2[0], buf);
  ASSERT(0 == strcmp(buf, "21"));
  hex_format_byte_as_hex(objs->test_data_2[1], buf);
  ASSERT(0 == strcmp(buf, "22"));
  hex_format_byte_as_hex(objs->test_data_2[2], buf);
  ASSERT(0 == strcmp(buf, "23"));
  hex_format_byte_as_hex(objs->test_data_2[3], buf);
  ASSERT(0 == strcmp(buf, "24"));
  hex_format_byte_as_hex(objs->test_data_2[4], buf);
  ASSERT(0 == strcmp(buf, "26"));
  hex_format_byte_as_hex(objs->test_data_2[5], buf);
  ASSERT(0 == strcmp(buf, "27"));
  hex_format_byte_as_hex(objs->test_data_2[6], buf);
  ASSERT(0 == strcmp(buf, "28"));
  hex_format_byte_as_hex(objs->test_data_2[7], buf);
  ASSERT(0 == strcmp(buf, "29"));
}

void testHexToPrintable(TestObjs *objs) {
  // Hello, world!
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
  ASSERT('e' == hex_to_printable(objs->test_data_1[1]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[2]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[3]));
  ASSERT('o' == hex_to_printable(objs->test_data_1[4]));
  ASSERT(',' == hex_to_printable(objs->test_data_1[5]));
  ASSERT(' ' == hex_to_printable(objs->test_data_1[6]));
  ASSERT('w' == hex_to_printable(objs->test_data_1[7]));
  ASSERT('o' == hex_to_printable(objs->test_data_1[8]));
  ASSERT('r' == hex_to_printable(objs->test_data_1[9]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[10]));
  ASSERT('d' == hex_to_printable(objs->test_data_1[11]));
  ASSERT('!' == hex_to_printable(objs->test_data_1[12]));
}
