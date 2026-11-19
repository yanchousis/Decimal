#include "test.h"

START_TEST(less_true_1) {
  my_decimal a = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};  // 5
  my_decimal b = {{0x00000008, 0x00000000, 0x00000000, 0x00000000}};  // 8
  ck_assert_int_eq(my_is_less(a, b), 1);
}
END_TEST

START_TEST(less_true_2) {
  my_decimal a = {{0x0000000A, 0x00000000, 0x00000000, 0x80000000}};  // -10
  my_decimal b = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};  // 5
  ck_assert_int_eq(my_is_less(a, b), 1);
}
END_TEST

START_TEST(less_false_1) {
  my_decimal a = {{0x00000008, 0x00000000, 0x00000000, 0x00000000}};  // 8
  my_decimal b = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};  // 5
  ck_assert_int_eq(my_is_less(a, b), 0);
}
END_TEST

START_TEST(less_false_2) {
  my_decimal a = {{0x00000007, 0x00000000, 0x00000000, 0x00000000}};  // 7
  my_decimal b = {{0x00000007, 0x00000000, 0x00000000, 0x00000000}};  // 7
  ck_assert_int_eq(my_is_less(a, b), 0);
}
END_TEST

START_TEST(less_true_negative) {
  my_decimal a = {{0x00000006, 0x00000000, 0x00000000, 0x80000000}};  // -6
  my_decimal b = {{0x00000004, 0x00000000, 0x00000000, 0x80000000}};  // -4
  ck_assert_int_eq(my_is_less(a, b), 1);
}
END_TEST

START_TEST(less_false_negative) {
  my_decimal a = {{0x00000002, 0x00000000, 0x00000000, 0x80000000}};  // -2
  my_decimal b = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}};  // -5
  ck_assert_int_eq(my_is_less(a, b), 0);
}
END_TEST

START_TEST(less_large_negative) {
  my_decimal a = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
                    0x80000000}};  // -52818775009509558395695966890
  my_decimal b = {{0x11111111, 0x11111111, 0x11111111,
                    0x80000000}};  // -5281877500950955839569596689
  ck_assert_int_eq(my_is_less(a, b), 1);
}
END_TEST

START_TEST(less_zero) {
  my_decimal a = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};  // 0
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};  // 0
  ck_assert_int_eq(my_is_less(a, b), 0);
}
END_TEST

Suite *test_is_less(void) {
  Suite *s = suite_create("\033[45m-=my_is_less=-\033[0m");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, less_true_1);
  tcase_add_test(tc_core, less_true_2);
  tcase_add_test(tc_core, less_false_1);
  tcase_add_test(tc_core, less_false_2);
  tcase_add_test(tc_core, less_true_negative);
  tcase_add_test(tc_core, less_false_negative);
  tcase_add_test(tc_core, less_large_negative);
  tcase_add_test(tc_core, less_zero);

  suite_add_tcase(s, tc_core);
  return s;
}
