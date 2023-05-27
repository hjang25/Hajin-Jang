/*
 * Tests that help determine the functionality of functions in uint256.c
 * Hajin Jang
 */

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"

#include "uint256.h"

typedef struct {
  UInt256 zero;
  UInt256 one;
  UInt256 large1;

  const char *hex1;
  const char *hex2;
  const char *hex3;
  const char *hex4;
  const char *hex5;
} TestObjs;

// Functions to create and cleanup the test fixture object
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// Helper functions for implementing tests
int check(UInt256 val, uint64_t val3, uint64_t val2, uint64_t val1, uint64_t val0);

// Declarations of test functions
void test_get_bits(TestObjs *objs);
void test_create_from_u64(TestObjs *objs);
void test_create(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_format_as_hex2();
void test_add_1(TestObjs *objs);
void test_add_2(TestObjs *objs);
void test_add_3(TestObjs *objs);
void test_add_4();
void test_add_5();
void test_sub_1(TestObjs *objs);
void test_sub_2(TestObjs *objs);
void test_sub_3(TestObjs *objs);
void test_sub_4();
void test_sub_5();
void test_sub_6();
void test_mul_1(TestObjs *objs);
void test_mul_2(TestObjs *objs);
void test_mul_3();
void test_mul_4();

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_get_bits);
  TEST(test_create_from_u64);
  TEST(test_create);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_format_as_hex2);
  TEST(test_add_1);
  TEST(test_add_2);
  TEST(test_add_3);
  TEST(test_add_4);
  TEST(test_add_5);
  TEST(test_sub_1);
  TEST(test_sub_2);
  TEST(test_sub_3);
  TEST(test_sub_4);
  TEST(test_sub_5);
  TEST(test_sub_6);
  TEST(test_mul_1);
  TEST(test_mul_2);
  TEST(test_mul_3);
  TEST(test_mul_4);

  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = (TestObjs *) malloc(sizeof(TestObjs));

  // initialize several UInt256 values "manually"

  objs->zero.data[3] = 0U;
  objs->zero.data[2] = 0U;
  objs->zero.data[1] = 0U;
  objs->zero.data[0] = 0U;

  objs->one.data[3] = 0U;
  objs->one.data[2] = 0U;
  objs->one.data[1] = 0U;
  objs->one.data[0] = 1U;

  objs->large1.data[3] = 0xAA;
  objs->large1.data[2] = 0xBB;
  objs->large1.data[1] = 0xCC;
  objs->large1.data[0] = 0xDD;

  // example hex strings
  objs->hex1 = "0";
  objs->hex2 = "cafe";

  // additional tests for 'uint256_create_from_hex'
  objs->hex3 = "88ac45b52f1bc4518868be20b3c67f9077a137e5f4858db633ab07f";
  objs->hex4 = "4984328e5bd74c6530b2fa6f0190e0e38164294540a3172228330160afa057fa";
  objs->hex5 = "c6a3fd1efa8a2152918ee8c7e35e808b582342018741ea726d32a4d6258fe348a6ecc4";

  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

int check(UInt256 val, uint64_t val3, uint64_t val2, uint64_t val1, uint64_t val0) {
  return uint256_get_bits(val, 3) == val3
      && uint256_get_bits(val, 2) == val2
      && uint256_get_bits(val, 1) == val1
      && uint256_get_bits(val, 0) == val0;
}

void test_get_bits(TestObjs *objs) {
  ASSERT(0U == uint256_get_bits(objs->zero, 3));
  ASSERT(0U == uint256_get_bits(objs->zero, 2));
  ASSERT(0U == uint256_get_bits(objs->zero, 1));
  ASSERT(0U == uint256_get_bits(objs->zero, 0));

  ASSERT(0U == uint256_get_bits(objs->one, 3));
  ASSERT(0U == uint256_get_bits(objs->one, 2));
  ASSERT(0U == uint256_get_bits(objs->one, 1));
  ASSERT(1U == uint256_get_bits(objs->one, 0));
}

void test_create_from_u64(TestObjs *objs) {
  objs->zero = uint256_create_from_u64(0U);
  objs->one = uint256_create_from_u64(1U);

  ASSERT(check(objs->zero, 0U, 0U, 0U, 0U));
  ASSERT(check(objs->one, 0U, 0U, 0U, 1U));
}

void test_create(TestObjs *objs) {
  // Note that the initializers list the uint64_t values to
  // be stored in order from least significant to most significant
  uint64_t zero_init[] =   {0U, 0U, 0U, 0U};
  uint64_t one_init[] =    {1U, 0U, 0U, 0U};

  objs->zero = uint256_create(zero_init);
  objs->one = uint256_create(one_init);

  // In the calls to check, the uint64_t values are specified
  // in the order most significant to least significant
  // (i.e., the reverse of the order in the array passed to
  // uint256_create)
  ASSERT(check(objs->zero, 0U, 0U, 0U, 0U));
  ASSERT(check(objs->one, 0U, 0U, 0U, 1U));
}

void test_create_from_hex(TestObjs *objs) {
  UInt256 val;

  // case 1: hex string of 0
  val = uint256_create_from_hex(objs->hex1);
  ASSERT(check(val, 0x0UL, 0x0UL, 0x0UL, 0x0UL));

  // case 2: hex string between 0 and 64 digits (e.g. 4 and 55)
  val = uint256_create_from_hex(objs->hex2);
  ASSERT(check(val, 0x0UL, 0x0UL, 0x0UL, 0xcafeUL));
  val = uint256_create_from_hex(objs->hex3);
  ASSERT(check(val, 0x88ac45bUL, 0x52f1bc4518868be2UL, 0x0b3c67f9077a137eUL, 0x5f4858db633ab07fUL));

  // case 3 : hex string with exactly 64 digits
  val = uint256_create_from_hex(objs->hex4);
  ASSERT(check(val, 0x4984328e5bd74c65UL, 0x30b2fa6f0190e0e3UL, 0x8164294540a31722UL, 0x28330160afa057faUL));

  // case 4: hex string with more than 64 digits (e.g. 70)
  val = uint256_create_from_hex(objs->hex5);
  ASSERT(check(val, 0x1efa8a2152918ee8UL, 0xc7e35e808b582342UL, 0x018741ea726d32a4UL, 0xd6258fe348a6ecc4UL));
}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = uint256_format_as_hex(objs->zero);
  ASSERT(0 == strcmp("0", s));
  free(s);

  s = uint256_format_as_hex(objs->one);
  ASSERT(0 == strcmp("1", s));
  free(s);
}

// test if 'uint256_format_as_hex' handles leading zeros correctly
void test_format_as_hex2() {
  char *str1, *str2, *str3;
  UInt256 u_obj1 = {0}, u_obj2 = {0}, u_obj3 = {0};

  u_obj1.data[1] = 1UL;
  str1 = uint256_format_as_hex(u_obj1);
  ASSERT(0 == strcmp("10000000000000000", str1));
  free(str1);

  u_obj2.data[2] = 1UL;
  str2 = uint256_format_as_hex(u_obj2);
  ASSERT(0 == strcmp("100000000000000000000000000000000", str2));
  free(str2);

  u_obj3.data[3] = 1UL;
  str3 = uint256_format_as_hex(u_obj3);
  ASSERT(0 == strcmp("1000000000000000000000000000000000000000000000000", str3));
  free(str3);
}

void test_add_1(TestObjs *objs) {
  // basic addition tests

  UInt256 sum;

  sum = uint256_add(objs->zero, objs->one);

  ASSERT(0UL == sum.data[3]);
  ASSERT(0UL == sum.data[2]);
  ASSERT(0UL == sum.data[1]);
  ASSERT(1UL == sum.data[0]);
}

void test_add_2(TestObjs *objs) {
  // "medium" addition tests

  (void) objs;

  UInt256 left, right, result;

  // a932467b27ccb2b4cb6c96c4810cb5e + b37aef92a5179d60af2f9cad0a4b38a = 15cad360dcce450157a9c33718b57ee8
  left.data[0] = 0x4cb6c96c4810cb5eUL;
  left.data[1] = 0xa932467b27ccb2bUL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0x0af2f9cad0a4b38aUL;
  right.data[1] = 0xb37aef92a5179d6UL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_add(left, right);
  ASSERT(0x57a9c33718b57ee8UL == result.data[0]);
  ASSERT(0x15cad360dcce4501UL == result.data[1]);
  ASSERT(0x0UL == result.data[2]);
  ASSERT(0x0UL == result.data[3]);
}

void test_add_3(TestObjs *objs) {
  // "large" addition tests

  (void) objs;

  UInt256 left, right, result;

  // 9515af0631ecc4779e0c122009e87b1ff1076115dab87f8190bc8c2ceb84ceb + 173ba2210b102e74dc94683c12d20ceda74af7d4cf5fc80aa154504e7e58712 = ac5151273cfcf2ec7aa07a5c1cba880d985258eaaa18478c3210dc7b69dd3fd
  left.data[0] = 0x190bc8c2ceb84cebUL;
  left.data[1] = 0xff1076115dab87f8UL;
  left.data[2] = 0x79e0c122009e87b1UL;
  left.data[3] = 0x9515af0631ecc47UL;
  right.data[0] = 0xaa154504e7e58712UL;
  right.data[1] = 0xda74af7d4cf5fc80UL;
  right.data[2] = 0x4dc94683c12d20ceUL;
  right.data[3] = 0x173ba2210b102e7UL;
  result = uint256_add(left, right);
  ASSERT(0xc3210dc7b69dd3fdUL == result.data[0]);
  ASSERT(0xd985258eaaa18478UL == result.data[1]);
  ASSERT(0xc7aa07a5c1cba880UL == result.data[2]);
  ASSERT(0xac5151273cfcf2eUL == result.data[3]);
}

void test_add_4() {
  // addition test with overflow (max number that can by represented + 1)

  UInt256 max;
  for (int i = 0; i < 4; ++i) { max.data[i] = ~(0UL); }

  UInt256 one = uint256_create_from_u64(1UL);

  UInt256 sum = uint256_add(max, one);

  ASSERT(sum.data[3] == 0UL);
  ASSERT(sum.data[2] == 0UL);
  ASSERT(sum.data[1] == 0UL);
  ASSERT(sum.data[0] == 0UL);
}

void test_add_5() {

  // adding zero to a value
  UInt256 zero = uint256_create_from_u64(0UL);
  UInt256 left;
  left.data[0] = 0x4e25d5b1b2113e94UL;
  left.data[1] = 0x578f5d94ba0f6d62UL;
  left.data[2] = 0xfbe2a366fa4e221cUL;
  left.data[3] = 0x6daf37ff9977926eUL;

  UInt256 sum = uint256_add(left, zero);

  ASSERT(sum.data[3] == left.data[3]);
  ASSERT(sum.data[2] == left.data[2]);
  ASSERT(sum.data[1] == left.data[1]);
  ASSERT(sum.data[0] == left.data[0]);
}

void test_sub_1(TestObjs *objs) {
  // basic subtraction tests

  UInt256 result;

  result = uint256_sub(objs->one, objs->zero);
  ASSERT(0UL == result.data[3]);
  ASSERT(0UL == result.data[2]);
  ASSERT(0UL == result.data[1]);
  ASSERT(1UL == result.data[0]);
}

void test_sub_2(TestObjs *objs) {
  // "medium" subtraction tests

  (void) objs;

  UInt256 left, right, result;

  // af7939833038cfa010f734368aaad66 - 6473865d77af47d71f7aedf64364960 = 4b05b325b88987c8f17c46404746406
  left.data[0] = 0x010f734368aaad66UL;
  left.data[1] = 0xaf7939833038cfaUL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0x71f7aedf64364960UL;
  right.data[1] = 0x6473865d77af47dUL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_sub(left, right);
  ASSERT(0x8f17c46404746406UL == result.data[0]);
  ASSERT(0x4b05b325b88987cUL == result.data[1]);
  ASSERT(0x0UL == result.data[2]);
  ASSERT(0x0UL == result.data[3]);
}

void test_sub_3(TestObjs *objs) {
  // "large" subtraction tests

  (void) objs;

  UInt256 left, right, result;

  // bc556287a225313cc07a1509f4ebb335034f5d413945ac7d0bdb42962a6ae8c - 7209ef9bebd10ecdc8fb3ccc6c9c69f41f2b217da808c18793c019c3cabaddc = 4a4b72ebb654226ef77ed83d884f4940e4243bc3913ceaf5781b28d25fb00b0
  left.data[0] = 0xd0bdb42962a6ae8cUL;
  left.data[1] = 0x5034f5d413945ac7UL;
  left.data[2] = 0xcc07a1509f4ebb33UL;
  left.data[3] = 0xbc556287a225313UL;
  right.data[0] = 0x793c019c3cabaddcUL;
  right.data[1] = 0x41f2b217da808c18UL;
  right.data[2] = 0xdc8fb3ccc6c9c69fUL;
  right.data[3] = 0x7209ef9bebd10ecUL;
  result = uint256_sub(left, right);
  ASSERT(0x5781b28d25fb00b0UL == result.data[0]);
  ASSERT(0x0e4243bc3913ceafUL == result.data[1]);
  ASSERT(0xef77ed83d884f494UL == result.data[2]);
  ASSERT(0x4a4b72ebb654226UL == result.data[3]);
}

void test_sub_4() {
  // test negative overflow (0 - 1 = max)

  UInt256 zero = uint256_create_from_u64(0UL);
  UInt256 one = uint256_create_from_u64(1UL);
  UInt256 max;
  for (int i = 0; i < 4; ++i) { max.data[i] = ~(0UL); }

  UInt256 sub = uint256_sub(zero, one);

  ASSERT(sub.data[3] == max.data[3]);
  ASSERT(sub.data[2] == max.data[2]);
  ASSERT(sub.data[1] == max.data[1]);
  ASSERT(sub.data[0] == max.data[0]);
}

void test_sub_5() {
  // subtracting zero from a value

  UInt256 zero = uint256_create_from_u64(0UL);
  UInt256 left;
  left.data[0] = 0x4e25d5b1b2113e94UL;
  left.data[1] = 0x578f5d94ba0f6d62UL;
  left.data[2] = 0xfbe2a366fa4e221cUL;
  left.data[3] = 0x6daf37ff9977926eUL;

  UInt256 sub = uint256_sub(left, zero);

  ASSERT(sub.data[3] == left.data[3]);
  ASSERT(sub.data[2] == left.data[2]);
  ASSERT(sub.data[1] == left.data[1]);
  ASSERT(sub.data[0] == left.data[0]);
}

void test_sub_6() {
  // random subtraction test

  UInt256 left, right, result;

  left.data[0] = 0xeee7d8c9a5f9e82cUL;
  left.data[1] = 0x39a280bf0772e5f0UL;
  left.data[2] = 0xf406fd64866ad2fbUL;
  left.data[3] = 0xc0b02ba840f40caUL;
  right.data[0] = 0x6e548f7b625a4748UL;
  right.data[1] = 0x1e9f16ff7730cfd4UL;
  right.data[2] = 0xcda8f5ed97995636UL;
  right.data[3] = 0x3f69093a9cdceceUL;
  result = uint256_sub(left, right);
  ASSERT(0x8093494e439fa0e4UL == result.data[0]);
  ASSERT(0x1b0369bf9042161cUL == result.data[1]);
  ASSERT(0x265e0776eed17cc5UL == result.data[2]);
  ASSERT(0x8147226da4171fcUL == result.data[3]);
}

void test_mul_1(TestObjs *objs) {
  // basic multiplication tests

  UInt256 result;

  result = uint256_mul(objs->one, objs->one);
  ASSERT(check(result, 0UL, 0UL, 0UL, 1UL));

  result = uint256_mul(objs->one, objs->zero);
  ASSERT(check(result, 0UL, 0UL, 0UL, 0UL));
}

void test_mul_2(TestObjs *objs) {
  (void) objs;

  UInt256 left, right, result;

  // 761544a98b82abc63f23766d1391782 * 14bf658bd8053a9484c32d955a47a2f = 991f2125eacd361abad710163aa9be6117fa57cddf52e73c97a28d7f744de
  left.data[0] = 0x63f23766d1391782UL;
  left.data[1] = 0x761544a98b82abcUL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0x484c32d955a47a2fUL;
  right.data[1] = 0x14bf658bd8053a9UL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_mul(left, right);
  ASSERT(0x73c97a28d7f744deUL == result.data[0]);
  ASSERT(0xe6117fa57cddf52eUL == result.data[1]);
  ASSERT(0x61abad710163aa9bUL == result.data[2]);
  ASSERT(0x991f2125eacd3UL == result.data[3]);
}

void test_mul_3() {
  // additional test for multiplication 1

  UInt256 left, right, result;
  
  // 7f6abe5d04cc416c8eae41cdcce964b * ff89645bb1971ca94e2fc8d27c66eb0 = 7f2fb5b1dd410b432dc4cc529a5080230cfcbb27fee51ee76816b53d9d8d90
  left.data[0] = 0xc8eae41cdcce964bUL;
  left.data[1] = 0x7f6abe5d04cc416UL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0x94e2fc8d27c66eb0UL;
  right.data[1] = 0xff89645bb1971caUL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_mul(left, right);
  ASSERT(0xe76816b53d9d8d90UL == result.data[0]);
  ASSERT(0x230cfcbb27fee51eUL == result.data[1]);
  ASSERT(0x432dc4cc529a5080UL == result.data[2]);
  ASSERT(0x7f2fb5b1dd410bUL == result.data[3]);
}

void test_mul_4() {
  // additional test for multiplication 2

  UInt256 left, right, result;

  // 355def2475d561d4c58ab7973fed6fe * c00789b2c3e1c98bf1138a19bad8078 = 280805a16b7fab5a42856869b19accc1ef66acf00ef98c04c6900ede99c710
  left.data[0] = 0x4c58ab7973fed6feUL;
  left.data[1] = 0x355def2475d561dUL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0xbf1138a19bad8078UL;
  right.data[1] = 0xc00789b2c3e1c98UL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_mul(left, right);
  ASSERT(0x04c6900ede99c710UL == result.data[0]);
  ASSERT(0xc1ef66acf00ef98cUL == result.data[1]);
  ASSERT(0x5a42856869b19accUL == result.data[2]);
  ASSERT(0x280805a16b7fabUL == result.data[3]);
}
