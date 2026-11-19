#include "test.h"

// from_int_to_decimal

START_TEST(test_my_from_int_to_decimal) {
  const int a = 15000;
  my_decimal result;
  my_initDecLMHScSign(&result, 0, 0, 0, 0, 0);
  const int arr[4] = {15000, 0, 0, 0};

  my_from_int_to_decimal(a, &result);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result.bits[i], arr[i]);
  }
}
END_TEST

START_TEST(test_my_from_negint_to_decimal) {
  const int a = -15000;
  my_decimal result;
  my_initDecLMHScSign(&result, 0, 0, 0, 0, 0);
  const int arr[4] = {15000, 0, 0, 1u << 31};

  my_from_int_to_decimal(a, &result);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result.bits[i], arr[i]);
  }
}
END_TEST

START_TEST(test_my_from_0_to_decimal) {
  const int a = 0;
  my_decimal result;
  my_initDecLMHScSign(&result, 0, 0, 0, 0, 0);
  const int arr[4] = {0, 0, 0, 0};

  my_from_int_to_decimal(a, &result);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result.bits[i], arr[i]);
  }
}
END_TEST

START_TEST(test_my_from_int_to_decimal_NULL) {
  my_decimal *result = NULL;

  ck_assert_int_eq(my_from_int_to_decimal(0, result), 1);
}
END_TEST

START_TEST(test_my_from_int_to_decimal_ret0) {
  const int a = 0;
  my_decimal result;
  my_initDecLMHScSign(&result, 0, 0, 0, 0, 0);

  ck_assert_int_eq(my_from_int_to_decimal(a, &result), 0);
}
END_TEST

// from_float_to_decimal

START_TEST(test_my_from_float_to_decimal_1) {
  const float a = 7.1;
  my_decimal result;
  my_initDecLMHScSign(&result, 0, 0, 0, 0, 0);
  const int arr[4] = {71, 0, 0, 1 << 16};

  my_from_float_to_decimal(a, &result);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result.bits[i], arr[i]);
  }
}
END_TEST

START_TEST(test_my_from_float_to_decimal_2) {
  const double a = 7.16546464;
  my_decimal result;
  my_initDecLMHScSign(&result, 0, 0, 0, 0, 0);
  const int arr[4] = {7165465, 0, 0, 0b110 << 16};

  my_from_float_to_decimal(a, &result);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result.bits[i], arr[i]);
  }
}
END_TEST

START_TEST(test_my_from_negfloat_to_decimal) {
  const float a = -7.1;
  my_decimal result;
  my_initDecLMHScSign(&result, 0, 0, 0, 0, 0);
  const int arr[4] = {71, 0, 0, 1u << 16 | 1u << 31};

  my_from_float_to_decimal(a, &result);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result.bits[i], arr[i]);
  }
}
END_TEST

START_TEST(test_my_from_float0_to_decimal) {
  const float a = 0.0;
  my_decimal result;
  my_initDecLMHScSign(&result, 0, 0, 0, 0, 0);
  const int arr[4] = {0, 0, 0, 0};

  my_from_float_to_decimal(a, &result);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result.bits[i], arr[i]);
  }
}
END_TEST

START_TEST(test_my_from_float_to_decimal_NULL) {
  my_decimal *result = NULL;

  ck_assert_int_eq(my_from_float_to_decimal(0, result), 1);
}
END_TEST

START_TEST(test_my_from_float_to_decimal_ret0) {
  const float a = 12.77;
  my_decimal result;
  my_initDecLMHScSign(&result, 0, 0, 0, 0, 0);

  ck_assert_int_eq(my_from_float_to_decimal(a, &result), 0);
}
END_TEST

// from_decimal_to_int

START_TEST(test_my_from_decimal_to_int) {
  int result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, 15000, 0, 0, 0, 0);
  const int expected = 15000;

  my_from_decimal_to_int(a, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_my_from_negdecimal_to_int) {
  int result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, 15000, 0, 0, 0, 1);
  const int expected = -15000;

  my_from_decimal_to_int(a, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_my_from_biggerdecimal_to_int) {
  int result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, 1, 1, 0, 9, 0);
  const int expected = 4;

  my_from_decimal_to_int(a, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_my_from_decimal_to_int_error) {
  int result = 0;
  my_decimal a = {{1, 1, 0, 0b1001 << 18}};

  ck_assert_int_eq(my_from_decimal_to_int(a, &result), 1);
}
END_TEST

START_TEST(test_my_from_decimal_to_int_error_2) {
  int result = 0;
  my_decimal a = {{1, 1, 0, 0b11111111 << 24}};

  ck_assert_int_eq(my_from_decimal_to_int(a, &result), 1);
}
END_TEST

START_TEST(test_my_from_decimal_to_int_error_3) {
  int result = 0;
  my_decimal a = {{-1, -1, -1, 0xFF0000}};

  ck_assert_int_eq(my_from_decimal_to_int(a, &result), 1);
}
END_TEST

START_TEST(test_my_from_decimal_to_int_error_4) {
  int result = 0;
  my_decimal a = {{0, 0, 0, 0xFF0000 + (1u << 31)}};

  ck_assert_int_eq(my_from_decimal_to_int(a, &result), 1);
}
END_TEST

START_TEST(test_my_from_decimal0_to_int) {
  int result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, 0, 0, 0, 0, 0);
  const int expected = 0;

  my_from_decimal_to_int(a, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_my_from_decimal_to_int_NULL) {
  int *result = NULL;
  my_decimal a = {0};

  ck_assert_int_eq(my_from_decimal_to_int(a, result), 1);
}
END_TEST

START_TEST(test_my_from_decimalmaxscalemax_to_int) {
  int result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, -1, -1, -1, 28, 0);
  const int expected = 7;

  my_from_decimal_to_int(a, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_my_from_decimalmax_to_int) {
  int result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, -1, -1, -1, 0, 0);
  const int expected = INT_MIN;

  my_from_decimal_to_int(a, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

// from_decimal_to_float

START_TEST(test_my_from_decimal_to_float) {
  float result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, 71, 0, 0, 1, 0);
  const float expected = 7.1;

  my_from_decimal_to_float(a, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(test_my_from_negdecimal_to_float) {
  float result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, 71, 0, 0, 1, 1);
  const float expected = -7.1;

  my_from_decimal_to_float(a, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(test_my_from_decimalint_to_float) {
  float result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, 71, 0, 0, 0, 0);
  const float expected = 71.0;

  my_from_decimal_to_float(a, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(test_my_from_decimal_to_float_error) {
  float result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, 71, 0, 0, 30, 0);

  ck_assert_int_eq(my_from_decimal_to_float(a, &result), 1);
}
END_TEST

START_TEST(test_my_from_decimal0_to_float) {
  float result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, 0, 0, 0, 0, 0);
  const float expected = 0.0;

  my_from_decimal_to_float(a, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(test_my_from_decimal_to_float_NULL) {
  float *result = NULL;
  my_decimal a = {0};

  ck_assert_int_eq(my_from_decimal_to_float(a, result), 1);
}
END_TEST

START_TEST(test_my_from_decimalmaxscale_to_float) {
  float result = 0;
  my_decimal a;
  my_initDecLMHScSign(&a, 1, 0, 0, 28, 0);
  const float expected = 1e-28f;

  my_from_decimal_to_float(a, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(test_fillFloatDecimal_overflow) {
  my_decimal dst;
  float src = 1e30;

  int res = my_fillFloatDecimal(src, &dst, 0, 0);

  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(float_to_decimal_round_up_8_digits) {
  float src = 123456790.0f;

  my_decimal expected;
  my_initDecLMHScSign(&expected, 123456800, 0, 0, 0, 0);

  my_decimal result;
  int res = my_from_float_to_decimal(src, &result);

  ck_assert_int_eq(res, 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(float_to_decimal_round_down_9_digits) {
  float src = 123456744.4f;

  my_decimal expected;
  my_initDecLMHScSign(&expected, 123456700, 0, 0, 0, 0);

  my_decimal result;
  int res = my_from_float_to_decimal(src, &result);

  ck_assert_int_eq(res, 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(test_exp_leveling_exp1_gt_28) {
  my_decimal num1;
  my_initDecLMHScSign(&num1, 123456789, 0, 0, 30, 0);  // scale 30
  my_decimal num2;
  my_initDecLMHScSign(&num2, 123456789, 0, 0, 2, 0);  // scale 2

  my_wideDecimal wideNum1 = my_decimalToWideDecimal(num1);
  my_wideDecimal wideNum2 = my_decimalToWideDecimal(num2);

  exp_leveling(&wideNum1, &wideNum2, num1, num2);

  ck_assert_int_ne(wideNum1.bits[0], 123456789);
}
END_TEST

START_TEST(test_exp_leveling_exp2_gt_28) {
  my_decimal num1;
  my_initDecLMHScSign(&num1, 123456789, 0, 0, 2, 0);  // scale 2
  my_decimal num2;
  my_initDecLMHScSign(&num2, 123456789, 0, 0, 30, 0);  // scale 30

  my_wideDecimal wideNum1 = my_decimalToWideDecimal(num1);
  my_wideDecimal wideNum2 = my_decimalToWideDecimal(num2);

  exp_leveling(&wideNum1, &wideNum2, num1, num2);

  ck_assert_int_ne(wideNum2.bits[0], 123456789);
}
END_TEST

Suite *test_converters(void) {
  Suite *s = suite_create("\033[45m-=my_converters=-\033[0m");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, float_to_decimal_round_up_8_digits);
  tcase_add_test(tc_core, float_to_decimal_round_down_9_digits);
  tcase_add_test(tc_core, test_fillFloatDecimal_overflow);

  tcase_add_test(tc_core, test_exp_leveling_exp1_gt_28);
  tcase_add_test(tc_core, test_exp_leveling_exp2_gt_28);

  tcase_add_test(tc_core, test_my_from_int_to_decimal);
  tcase_add_test(tc_core, test_my_from_negint_to_decimal);
  tcase_add_test(tc_core, test_my_from_0_to_decimal);
  tcase_add_test(tc_core, test_my_from_int_to_decimal_NULL);
  tcase_add_test(tc_core, test_my_from_int_to_decimal_ret0);

  tcase_add_test(tc_core, test_my_from_float_to_decimal_1);
  tcase_add_test(tc_core, test_my_from_float_to_decimal_2);
  tcase_add_test(tc_core, test_my_from_negfloat_to_decimal);
  tcase_add_test(tc_core, test_my_from_float0_to_decimal);
  tcase_add_test(tc_core, test_my_from_float_to_decimal_NULL);
  tcase_add_test(tc_core, test_my_from_float_to_decimal_ret0);

  tcase_add_test(tc_core, test_my_from_decimal_to_int);
  tcase_add_test(tc_core, test_my_from_negdecimal_to_int);
  tcase_add_test(tc_core, test_my_from_biggerdecimal_to_int);
  tcase_add_test(tc_core, test_my_from_decimal_to_int_error);
  tcase_add_test(tc_core, test_my_from_decimal0_to_int);
  tcase_add_test(tc_core, test_my_from_decimal_to_int_NULL);
  tcase_add_test(tc_core, test_my_from_decimalmaxscalemax_to_int);
  tcase_add_test(tc_core, test_my_from_decimalmax_to_int);
  tcase_add_test(tc_core, test_my_from_decimal_to_int_error_2);
  tcase_add_test(tc_core, test_my_from_decimal_to_int_error_3);
  tcase_add_test(tc_core, test_my_from_decimal_to_int_error_4);

  tcase_add_test(tc_core, test_my_from_decimal_to_float);
  tcase_add_test(tc_core, test_my_from_negdecimal_to_float);
  tcase_add_test(tc_core, test_my_from_decimalint_to_float);
  tcase_add_test(tc_core, test_my_from_decimal_to_float_error);
  tcase_add_test(tc_core, test_my_from_decimal0_to_float);
  tcase_add_test(tc_core, test_my_from_decimal_to_float_NULL);
  tcase_add_test(tc_core, test_my_from_decimalmaxscale_to_float);

  suite_add_tcase(s, tc_core);
  return s;
}