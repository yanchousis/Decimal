#include "test.h"

START_TEST(sub_normal_numbers_1) {
  my_decimal a = {{5, 0, 0, 0}};
  my_decimal b = {{4, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{1, 0, 0, 0}};
  ck_assert_int_eq(my_sub(a, b, &result), 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(sub_normal_numbers_2) {
  my_decimal a = {{5, 0, 0, 0}};
  my_decimal b = {{4, 0, 0, 1 << 17}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0b111110000, 0, 0, 1 << 17}};

  ck_assert_int_eq(my_sub(a, b, &result), 0);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(sub_large_numbers_overflow_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  my_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_sub(a, b, &result);
  ck_assert_int_eq(res_code, 1);
}
END_TEST

START_TEST(sub_small_numbers_underflow_1) {
  my_decimal a = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  my_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_sub(a, b, &result);
  ck_assert_int_eq(res_code, 0);
}
END_TEST

START_TEST(sub_zero) {
  my_decimal a = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_sub(a, b, &result);
  ck_assert_int_eq(res_code, 0);
  my_decimal expected = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(sub_neg_numbers) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  my_decimal b = {{0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE, 0x80000000}};
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_sub(a, b, &result);
  ck_assert_int_eq(res_code, 0);
}
END_TEST

START_TEST(sub_pos_and_neg) {
  my_decimal a = {{0x00000010, 0x00000000, 0x00000000, 0x00000000}};
  my_decimal b = {{0x00000010, 0x00000000, 0x00000000, 0x80000000}};
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_sub(a, b, &result);
  ck_assert_int_eq(res_code, 0);
  my_decimal expected = {{0x00000020, 0x00000000, 0x00000000, 0x00000000}};
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

Suite *test_sub(void) {
  Suite *s = suite_create("\033[45m-=my_sub=-\033[0m");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, sub_normal_numbers_1);
  tcase_add_test(tc_core, sub_normal_numbers_2);
  tcase_add_test(tc_core, sub_large_numbers_overflow_1);
  tcase_add_test(tc_core, sub_small_numbers_underflow_1);
  tcase_add_test(tc_core, sub_zero);
  tcase_add_test(tc_core, sub_neg_numbers);
  tcase_add_test(tc_core, sub_pos_and_neg);
  suite_add_tcase(s, tc_core);
  return s;
}