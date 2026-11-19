#include "test.h"

START_TEST(equal_true_1) {
  my_decimal a = {{0x00000007, 0x00000000, 0x00000000, 0x00000000}};  // 7
  my_decimal b = {{0x00000007, 0x00000000, 0x00000000, 0x00000000}};  // 7
  ck_assert_int_eq(my_is_equal(a, b), 1);
}
END_TEST

START_TEST(equal_true_2) {
  my_decimal a = {{0x00000000, 0x00000000, 0x00000000,
                    0x801C0000}};  // -0.0000000000000000000000000000
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000,
                    0x801C0000}};  // -0.0000000000000000000000000000
  ck_assert_int_eq(my_is_equal(a, b), 1);
}
END_TEST

START_TEST(equal_false_1) {
  my_decimal a = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};  // 5
  my_decimal b = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}};  // 10
  ck_assert_int_eq(my_is_equal(a, b), 0);
}
END_TEST

START_TEST(equal_false_2) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x80000000}};  // -79228162514264337593543950335
  ck_assert_int_eq(my_is_equal(a, b), 0);
}
END_TEST

START_TEST(equal_false_3) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x80000000}};  // -79228162514264337593543950335
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000,
                    0x801C0000}};  // -0.0000000000000000000000000000
  ck_assert_int_eq(my_is_equal(a, b), 0);
}
END_TEST

START_TEST(equal_zero) {
  my_decimal a = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};  // 0
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};  // 0
  ck_assert_int_eq(my_is_equal(a, b), 1);
}
END_TEST

START_TEST(equal_different_scales_1) {
  my_decimal a = {{15, 0, 0, 0x10000}};   // 1.5
  my_decimal b = {{150, 0, 0, 0x20000}};  // 1.50
  ck_assert_int_eq(my_is_equal(a, b), 1);
}
END_TEST

START_TEST(equal_max_scale) {
  my_decimal a = {{1, 0, 0, 0x1C0000}};  // 0.0000000000000000000000000001
  my_decimal b = {{1, 0, 0, 0x1C0000}};  // 0.0000000000000000000000000001
  ck_assert_int_eq(my_is_equal(a, b), 1);
}
END_TEST

START_TEST(equal_zero_and_negative_zero) {
  my_decimal a = {{0, 0, 0, 0x00000000}};  // 0
  my_decimal b = {{0, 0, 0, 0x80000000}};  // -0
  ck_assert_int_eq(my_is_equal(a, b), 1);
}
END_TEST

Suite *test_is_equal(void) {
  Suite *s = suite_create("\033[45m-=my_is_equal=-\033[0m");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, equal_true_1);
  tcase_add_test(tc_core, equal_true_2);
  tcase_add_test(tc_core, equal_false_1);
  tcase_add_test(tc_core, equal_false_2);
  tcase_add_test(tc_core, equal_false_3);
  tcase_add_test(tc_core, equal_zero);
  tcase_add_test(tc_core, equal_different_scales_1);
  tcase_add_test(tc_core, equal_max_scale);
  tcase_add_test(tc_core, equal_zero_and_negative_zero);

  suite_add_tcase(s, tc_core);
  return s;
}