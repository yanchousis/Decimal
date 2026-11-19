#include "test.h"

// -79228162514264337593543950335 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
// 0x80000000}}

START_TEST(add_normal_numbers_1) {
  my_decimal a = {{5, 0, 0, 0}};
  my_decimal b = {{4, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{9, 0, 0, 0}};
  ck_assert_int_eq(my_add(a, b, &result), 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(add_max_scale) {
  my_decimal a = {
      {1, 0x00000000, 0x00000000, 0x1C0000}};  // 0.0000000000000000000000000001
  my_decimal b = {
      {1, 0x00000000, 0x00000000, 0x1C0000}};  // 0.0000000000000000000000000001
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {
      {2, 0, 0, 0x1C0000}};  // 0.0000000000000000000000000002
  ck_assert_int_eq(my_add(a, b, &result), 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(add_diff_signs_and_scales) {
  my_decimal a = {{123456, 0x00000000, 0x00000000, 0x50000}};  // 1.23456
  my_decimal b = {{789, 0x00000000, 0x00000000, 0x80030000}};  // -0.789
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{44556, 0, 0, 0x50000}};  // 0.44556
  ck_assert_int_eq(my_add(a, b, &result), 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(add_large_numbers_overflow_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }

  ck_assert_int_eq(res_code, 1);
}
END_TEST

START_TEST(add_large_numbers_overflow_2) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  my_decimal b = {{0x88000001, 0x1F128130, 0x1027E72F,
                    0x1C000000}};  // 0.5000000000000000000000000001
  my_decimal result = {{0, 0, 0, 0}};

  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 1);
}

START_TEST(add_large_numbers_overflow_3) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {{0xF555887, 0x553F1641, 0xB5597F0B,
                    0x60000000}};  // 56124981125209321576924.010631
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 1);
}

START_TEST(add_small_numbers_underflow_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x80000000}};  // -79228162514264337593543950335
  my_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x80000000}};  // -79228162514264337593543950335
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 2);
}

START_TEST(add_too_small_numbers_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x80000000}};  // -79228162514264337593543950335
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000,
                    0x801C0000}};  // -0.0000000000000000000000000000
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 0);
}

START_TEST(add_too_small_numbers_2) {
  my_decimal a = {{0x00000000, 0x00000000, 0x00000000, 0x00010000}};
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000, 0x00010000}};
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 0);
}
END_TEST

START_TEST(add_too_small_numbers_3) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x80000000}};  // -79228162514264337593543950335
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000,
                    0x801C0000}};  // -0.0000000000000000000000000000
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 0);
  my_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}

START_TEST(add_zero) {
  my_decimal a = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};  // 0
  my_decimal b = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};  // 5
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 0);
  my_decimal expected = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(add_wrong_scale) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1 << 22}};
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000, 1 << 22}};
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 2);
  my_decimal expected = {{0, 0, 0, 0}};

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}

START_TEST(add_wrong_scale_2) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1 << 22 | 1 << 23}};
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000, 1 << 22 | 1 << 23}};
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 1);
  my_decimal expected = {{0, 0, 0, 0}};

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}

START_TEST(add_neg_numbers) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};  // -1
  my_decimal b = {{0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE, 0x00000000}};  // -2
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 1);
}
END_TEST

START_TEST(add_pos_and_neg) {
  my_decimal a = {{0x00000010, 0x00000000, 0x00000000, 0x00000000}};  // 16
  my_decimal b = {{0x00000010, 0x00000000, 0x00000000, 0x80000000}};  // -16
  my_decimal result = {{0, 0, 0, 0}};
  int res_code = my_add(a, b, &result);

  ck_assert_int_eq(res_code, 0);
  my_decimal expected = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

Suite *test_add(void) {
  Suite *s = suite_create("\033[45m-=my_add=-\033[0m");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, add_normal_numbers_1);
  tcase_add_test(tc_core, add_max_scale);
  tcase_add_test(tc_core, add_diff_signs_and_scales);
  tcase_add_test(tc_core, add_large_numbers_overflow_1);
  tcase_add_test(tc_core, add_large_numbers_overflow_2);
  tcase_add_test(tc_core, add_large_numbers_overflow_3);
  tcase_add_test(tc_core, add_small_numbers_underflow_1);
  tcase_add_test(tc_core, add_too_small_numbers_1);
  tcase_add_test(tc_core, add_too_small_numbers_2);
  tcase_add_test(tc_core, add_too_small_numbers_3);
  tcase_add_test(tc_core, add_zero);
  tcase_add_test(tc_core, add_neg_numbers);
  tcase_add_test(tc_core, add_pos_and_neg);
  tcase_add_test(tc_core, add_wrong_scale);
  tcase_add_test(tc_core, add_wrong_scale_2);

  suite_add_tcase(s, tc_core);
  return s;
}
