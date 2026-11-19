#include "test.h"

START_TEST(mul_normal_numbers_1) {
  my_decimal a = {{5, 0, 0, 0}};
  my_decimal b = {{4, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{20, 0, 0, 0}};
  ck_assert_int_eq(my_mul(a, b, &result), 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(mul_normal_numbers_2) {
  my_decimal a = {{5, 0, 0, 0}};
  my_decimal b = {{4, 0, 0, 1 << 17}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{2, 0, 0, 1 << 16}};
  ck_assert_int_eq(my_mul(a, b, &result), 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(mul_large_numbers_overflow_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};  // 2
  my_decimal result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  int res_code = my_mul(a, b, &result);
  ck_assert_int_eq(res_code, 1);
}
END_TEST

START_TEST(mul_by_zero) {
  my_decimal a = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};  // 0
  my_decimal b = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};  // 5
  my_decimal result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  int res_code = my_mul(a, b, &result);
  ck_assert_int_eq(res_code, 0);
  my_decimal expected = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(mul_neg_numbers_underflow_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x80000000}};  // -79228162514264337593543950335
  my_decimal b = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};  // 2
  my_decimal result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  int res_code = my_mul(a, b, &result);
  ck_assert_int_eq(res_code, 2);
}
END_TEST

START_TEST(mul_pos_and_neg) {
  my_decimal a = {{0x00000010, 0x00000000, 0x00000000, 0x00000000}};  // 16
  my_decimal b = {{0x00000010, 0x00000000, 0x00000000, 0x80000000}};  // -16
  my_decimal result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  int res_code = my_mul(a, b, &result);
  ck_assert_int_eq(res_code, 0);
  my_decimal expected = {
      {0x00000100, 0x00000000, 0x00000000, 0x80000000}};  // -256
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

Suite *test_mul(void) {
  Suite *s = suite_create("\033[45m-=my_mul=-\033[0m");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, mul_normal_numbers_1);
  tcase_add_test(tc_core, mul_normal_numbers_2);
  tcase_add_test(tc_core, mul_large_numbers_overflow_1);
  tcase_add_test(tc_core, mul_by_zero);
  tcase_add_test(tc_core, mul_neg_numbers_underflow_1);
  tcase_add_test(tc_core, mul_pos_and_neg);

  suite_add_tcase(s, tc_core);
  return s;
}