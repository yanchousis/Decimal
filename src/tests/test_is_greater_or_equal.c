#include "test.h"

START_TEST(greater_or_equal_true_1) {
  my_decimal a = {{0x00000008, 0x00000000, 0x00000000, 0x00000000}};  // 8
  my_decimal b = {{0x00000008, 0x00000000, 0x00000000, 0x00000000}};  // 8
  ck_assert_int_eq(my_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(greater_or_equal_true_2) {
  my_decimal a = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}};  // 10
  my_decimal b = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};  // 5
  ck_assert_int_eq(my_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(greater_or_equal_false_1) {
  my_decimal a = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}};  // 3
  my_decimal b = {{0x00000007, 0x00000000, 0x00000000, 0x00000000}};  // 7
  ck_assert_int_eq(my_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(greater_or_equal_false_2) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x80000000}};  // -79228162514264337593543950335
  my_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  ck_assert_int_eq(my_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(greater_or_equal_true_neg) {
  my_decimal a = {{0x00000004, 0x00000000, 0x00000000, 0x80000000}};  // -4
  my_decimal b = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}};  // -5
  ck_assert_int_eq(my_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(greater_or_equal_false_neg) {
  my_decimal a = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}};  // -5
  my_decimal b = {{0x00000002, 0x00000000, 0x00000000, 0x80000000}};  // -2
  ck_assert_int_eq(my_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(greater_or_equal_large_neg) {
  my_decimal a = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
                    0x80000000}};  // -52818775009509558395695966890
  my_decimal b = {{0x11111111, 0x11111111, 0x11111111,
                    0x80000000}};  // -5281877500950955839569596689
  ck_assert_int_eq(my_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(greater_or_equal_zero) {
  my_decimal a = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};  // 0
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};  // 0
  ck_assert_int_eq(my_is_greater_or_equal(a, b), 1);
}
END_TEST

Suite *test_is_greater_or_equal(void) {
  Suite *s = suite_create("\033[45m-=my_is_greater_or_equal=-\033[0m");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, greater_or_equal_true_1);
  tcase_add_test(tc_core, greater_or_equal_true_2);
  tcase_add_test(tc_core, greater_or_equal_false_1);
  tcase_add_test(tc_core, greater_or_equal_false_2);
  tcase_add_test(tc_core, greater_or_equal_true_neg);
  tcase_add_test(tc_core, greater_or_equal_false_neg);
  tcase_add_test(tc_core, greater_or_equal_large_neg);
  tcase_add_test(tc_core, greater_or_equal_zero);

  suite_add_tcase(s, tc_core);
  return s;
}