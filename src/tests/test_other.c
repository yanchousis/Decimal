#include "test.h"

// floor test

START_TEST(floor_positive) {
  my_decimal a = {{0x1E240, 0x0, 0x0, 0x30000}};  // 123.456
  my_decimal expected = {{123, 0x0, 0x0, 0x0}};   // 123
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_floor(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(floor_big_1) {
  my_decimal a = {{0b01110111101010100011001000100100, 0b10111000, 0,
                    2u << 16}};  // 7922816251.24
  my_decimal expected = {
      {0b11011000001111001001010011111011, 0b1, 0, 0}};  // 7922816251
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_floor(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(floor_big_2) {
  my_decimal a = {
      {-1, -1, -1,
       5u << 16 | 1u << 31}};  // 79 228 162 514 264 337 593 543 950 335
  my_decimal expected = {
      {0b10000100001000110000111111010000, 0b10101100010001110001101101000111,
       0b1010011111000101, 1u << 31}};  // 79 228 162 514 264 337 593 544 0
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_floor(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(floor_error_exp) {
  my_decimal a = {{0b01110111101010100011001000100100, 0b10111000, 0,
                    30 << 16}};  // 792281625124 / 10^30
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_floor(a, &result), 1);
}
END_TEST

START_TEST(floor_max_maxscale) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    28u << 16}};  // 79 228 162 514 264 337 593 543 950 335
  my_decimal expected = {{7, 0, 0, 0}};  // 7
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_floor(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(floor_negative) {
  my_decimal a = {{0x1E240, 0x0, 0x0, 0x80030000}};     // -123.456
  my_decimal expected = {{124, 0x0, 0x0, 0x80000000}};  // -124
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_floor(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(floor_noscale) {
  my_decimal a = {{8526, 0x0, 0x0, 0x0}};         // 8526
  my_decimal expected = {{8526, 0x0, 0x0, 0x0}};  // 8526
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_floor(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(floor_error) {
  my_decimal a = {{0, 0, 0, 546489}};

  ck_assert_int_eq(my_floor(a, &a), 1);
}
END_TEST

// round test

START_TEST(round_positive) {
  my_decimal a = {{0x1E240, 0x0, 0x0, 0x30000}};  // 123.456
  my_decimal expected = {{123, 0x0, 0x0, 0x0}};   // 123
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_big_1) {
  my_decimal a = {{0b01010100000010111110010000000000, 0b10, 0x0,
                    9u << 16}};  // 10000000000
  my_decimal expected = {{10, 0x0, 0x0, 0x0}};
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_big_3) {
  my_decimal a = {
      {-1, -1, -1, 2u << 16}};  // 79 228 162 514 264 337 593 543 950 3 35
  my_decimal expected = {{0b00101000111101011100001010001111,
                           0b11110101110000101000111101011100,
                           0b10100011110101110000101000,
                           0}};  // 79 228 162 514 264 337 593 543 950 3
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_big_4) {
  my_decimal a = {
      {-1, -1, -1, 5u << 16}};  // 79 228 162 514 264 337 593 543 950 335
  my_decimal expected = {
      {0b10000100001000110000111111010000, 0b10101100010001110001101101000111,
       0b1010011111000101, 0}};  // 79 228 162 514 264 337 593 544 0
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_big_negative) {
  my_decimal a = {
      {-1, -1, -1,
       5u << 16 | 1u << 31}};  // 79 228 162 514 264 337 593 543 950 335
  my_decimal expected = {
      {0b10000100001000110000111111010000, 0b10101100010001110001101101000111,
       0b1010011111000101, 1u << 31}};  // 79 228 162 514 264 337 593 544 0
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_big_5) {
  my_decimal a = {
      {0, 0, 1u << 31, 1u << 16}};  // 39 614 081 257 132 168 796 771 975 168
  my_decimal expected = {{0b11001100110011001100110011001101,
                           0b11001100110011001100110011001100,
                           0b1100110011001100110011001100,
                           0}};  //   39 614 081 257 132 168 796 771 975 17
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_big_6) {
  my_decimal a = {
      {0, 0, 1u << 31, 28u << 16}};  // 39 614 081 257 132 168 796 771 975 168
  my_decimal expected = {
      {4, 0, 0, 0}};  //   39 614 081 257 132 168 796 771 975 17
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_max_maxscale) {
  my_decimal a = {
      {-1, -1, -1, 28u << 16}};  // 79 228 162 514 264 337 593 543 950 335
  my_decimal expected = {{8, 0, 0, 0}};  // 8
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_big_2) {
  my_decimal a = {
      {10, 0x0, 0xFFFFFFFF, 9u << 16}};  // 79228162495817593519 834398730
  my_decimal expected = {{0b001101010001000100011001010110000,
                           0b01001011100000101111101000000101, 0b100,
                           0x0}};  //      79228162495817593520
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_negative) {
  my_decimal a = {{0x1E240, 0x0, 0x0, 0x80030000}};     // -123.456
  my_decimal expected = {{123, 0x0, 0x0, 0x80000000}};  // -123
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_test) {
  my_decimal a = {{-1, -1, -1, 20u << 16}};
  my_decimal expected = {{0, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 0);
}
END_TEST

START_TEST(round_noscale) {
  my_decimal a = {{125, 0, 0, 0}};         // 12.5
  my_decimal expected = {{125, 0, 0, 0}};  // -13
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_round(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(round_error) {
  my_decimal a = {{0, 0, 0, 546489}};

  ck_assert_int_eq(my_round(a, &a), 1);
}
END_TEST
// truncate test

START_TEST(truncate_positive) {
  my_decimal a = {{0x1E240, 0x0, 0x0, 0x30000}};  // 123.456
  my_decimal expected = {{123, 0x0, 0x0, 0x0}};   // 123
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_truncate(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(truncate_negative) {
  my_decimal a = {{0x1E240, 0x0, 0x0, 0x80030000}};     // -123.456
  my_decimal expected = {{123, 0x0, 0x0, 0x80000000}};  // -123
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_truncate(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(truncate_noscale) {
  my_decimal a = {{1239, 0x0, 0x0, 0x0}};         // 1239
  my_decimal expected = {{1239, 0x0, 0x0, 0x0}};  // 1239
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_truncate(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(truncate_error) {
  my_decimal a = {{0, 0, 0, 546489}};

  ck_assert_int_eq(my_truncate(a, &a), 1);
}
END_TEST

// negative test

START_TEST(negate_positive) {
  my_decimal a = {{0x1E240, 0x0, 0x0, 0x30000}};            // 123.456
  my_decimal expected = {{0x1E240, 0x0, 0x0, 0x80030000}};  // -123.456
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_negate(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(negate_negative) {
  my_decimal a = {{0x1E240, 0x0, 0x0, 0x80030000}};      // -123.456
  my_decimal expected = {{0x1E240, 0x0, 0x0, 0x30000}};  // 123.456
  my_decimal result = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_negate(a, &result), 0);
  ck_assert_int_eq(my_is_equal(result, expected), 1);
}
END_TEST

START_TEST(negate_error) {
  my_decimal a = {{0x0, 0x0, 0x0, 5688}};  // 123.456

  ck_assert_int_eq(my_negate(a, &a), 1);
}
END_TEST

Suite *test_other(void) {
  Suite *s = suite_create("\033[45m-=my_other_functions=-\033[0m");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, floor_positive);
  tcase_add_test(tc_core, floor_big_1);
  tcase_add_test(tc_core, floor_big_2);
  tcase_add_test(tc_core, floor_error_exp);
  tcase_add_test(tc_core, floor_max_maxscale);
  tcase_add_test(tc_core, floor_negative);
  tcase_add_test(tc_core, floor_error);
  tcase_add_test(tc_core, floor_noscale);

  tcase_add_test(tc_core, round_positive);
  tcase_add_test(tc_core, round_max_maxscale);
  tcase_add_test(tc_core, round_negative);
  tcase_add_test(tc_core, round_test);
  tcase_add_test(tc_core, round_big_1);
  tcase_add_test(tc_core, round_big_negative);
  tcase_add_test(tc_core, round_big_2);
  tcase_add_test(tc_core, round_big_3);
  tcase_add_test(tc_core, round_big_4);
  tcase_add_test(tc_core, round_big_5);
  tcase_add_test(tc_core, round_big_6);
  tcase_add_test(tc_core, round_error);
  tcase_add_test(tc_core, round_noscale);
  tcase_add_test(tc_core, round_test);

  tcase_add_test(tc_core, truncate_positive);
  tcase_add_test(tc_core, truncate_negative);
  tcase_add_test(tc_core, truncate_error);
  tcase_add_test(tc_core, truncate_noscale);
  tcase_add_test(tc_core, negate_positive);
  tcase_add_test(tc_core, negate_negative);
  tcase_add_test(tc_core, negate_error);

  suite_add_tcase(s, tc_core);
  return s;
}