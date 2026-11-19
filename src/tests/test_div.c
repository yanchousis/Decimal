#include "test.h"

START_TEST(div_pos_numbers_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {{0xFFFFFFFF, 0x00000000, 0x00000000,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0x1, 0x1, 0x1, 0x0}};

  ck_assert_int_eq(my_div(a, b, &result), OK);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_pos_numbers_2) {
  my_decimal a = {{12367, 0, 0, 0}};
  my_decimal b = {{8, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{1545875, 0, 0, 3u << 16}};

  ck_assert_int_eq(my_div(a, b, &result), OK);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_pos_numbers_3) {
  my_decimal a = {
      {0b11001011101001101001101010110011, 0b11100, 0, 0x00000000}};
  my_decimal b = {{8, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {
      {0b01110000010110011000100101100111, 0b111000001111, 0, 3u << 16}};

  ck_assert_int_eq(my_div(a, b, &result), OK);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_pos_numbers_4) {
  my_decimal a = {{0, 1, 0, 0x00000000}};  // 17179869184
  my_decimal b = {{0, 4, 0, 0}};           // 4294967296
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{25, 0, 0, 2u << 16}};

  ck_assert_int_eq(my_div(a, b, &result), OK);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_big_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  my_decimal b = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  ck_assert_int_eq(my_div(a, b, &result), OK);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_big_2) {
  my_decimal a = {{0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000}};
  my_decimal b = {{2, 0x00000000, 0x00000000, 0x00000000}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {
      {0b11111111111111111111111111111011, 0b00000100, 0, 1u << 16}};

  ck_assert_int_eq(my_div(a, b, &result), OK);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_big_10) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  my_decimal b = {{10, 0x00000000, 0x00000000, 0x00000000}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1u << 16}};

  ck_assert_int_eq(my_div(a, b, &result), OK);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_neg_numbers) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x80000000}};  // -79228162514264337593543950335
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0x1, 0x0, 0x0, 0x80000000}};
  ck_assert_int_eq(my_div(a, b, &result), OK);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_big_numbers_overflow_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {{0x1, 0x0, 0x0, 0x1C0000}};
  // -0.0000000000000000000000000001
  my_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(my_div(a, b, &result), BIGNUMBER);
}
END_TEST

START_TEST(div_small_numbers_underflow_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {
      {0x1, 0x0, 0x0, 0x801C0000}};  // -0.0000000000000000000000000001
  my_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(my_div(a, b, &result), LOWNUMBER);
}
END_TEST

START_TEST(div_small_numbers_underflow_2) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {{0x80000000, 0x00000000, 0x00000000,
                    0x801C0000}};  // -0.0000000000000000000000000008
  my_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(my_div(a, b, &result), LOWNUMBER);
}
END_TEST

START_TEST(div_by_zero_1) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}};  // -0
  my_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(my_div(a, b, &result), DIVBYZERO);
}

START_TEST(div_by_zero_2) {
  my_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0x00000000}};  // 79228162514264337593543950335
  my_decimal b = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};  // 0
  my_decimal result = {{0, 0, 0, 0}};

  ck_assert_int_eq(my_div(a, b, &result), DIVBYZERO);
}

START_TEST(div_max_scale) {
  my_decimal a = {{1, 0, 0, 0x1C0000}};  // 0.0000000000000000000000000001
  my_decimal b = {{1, 0, 0, 0x1C0000}};  // 0.0000000000000000000000000001
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{1, 0, 0, 0x00000000}};  // 1.0
  ck_assert_int_eq(my_div(a, b, &result), OK);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(test_div_1) {
  my_decimal a = {{255, 0, 0, 0}};
  my_decimal b = {{240, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{10625, 0, 0, 1u << 18}};
  ck_assert_int_eq(my_div(a, b, &result), OK);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_max_min) {
  my_decimal a = {{-1, -1, -1, 0}};
  my_decimal b = {{1, 0, 0, 28u << 16}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_div(a, b, &result), BIGNUMBER);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_0_num) {
  my_decimal a = {{0, 0, 0, 0}};
  my_decimal b = {{1, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_div(a, b, &result), OK);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_0_0) {
  my_decimal a = {{0, 0, 0, 0}};
  my_decimal b = {{0, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(my_div(a, b, &result), DIVBYZERO);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_1_2) {
  my_decimal a = {{1, 0, 0, 0}};
  my_decimal b = {{2, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{5, 0, 0, 1u << 16}};
  ck_assert_int_eq(my_div(a, b, &result), OK);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_1_3) {
  my_decimal a = {{1, 0, 0, 0}};
  my_decimal b = {{3, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0b00000101010101010101010101010101,
                           0b00010100101101110000000011001011,
                           0b00001010110001010100010011001010, 28 << 16}};
  ck_assert_int_eq(my_div(a, b, &result), OK);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_2_3) {
  my_decimal a = {{2, 0, 0, 0}};
  my_decimal b = {{3, 0, 0, 0}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{0b00001010101010101010101010101010,
                           0b00101001011011100000000110010110,
                           0b10101100010101000100110010100, 28 << 16}};
  ck_assert_int_eq(my_div(a, b, &result), OK);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_simple_10_04) {
  my_decimal a = {{10, 0, 0, 0}};
  my_decimal b = {{4, 0, 0, 1u << 16}};
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{25, 0, 0, 0}};
  ck_assert_int_eq(my_div(a, b, &result), OK);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_scale5_to_scale2) {
  my_decimal a = {{2417888, 0, 0, 5u << 16}};  // 24.17888
  my_decimal b = {{128, 0, 0, 2u << 16}};      // 1.28
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{1888975, 0, 0, 5u << 16}};  // 18.88975
  ck_assert_int_eq(my_div(a, b, &result), OK);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(div_scale1_to_scale5) {
  my_decimal a = {{555, 0, 0, 1u << 16}};  // 55.5
  my_decimal b = {{5, 0, 0, 5u << 16}};    // 0.00005
  my_decimal result = {{0, 0, 0, 0}};
  my_decimal expected = {{1110000, 0, 0, 0}};  // 1110000
  ck_assert_int_eq(my_div(a, b, &result), OK);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

Suite *test_div(void) {
  Suite *s = suite_create("\033[45m-=my_div=-\033[0m");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, div_simple_10_04);
  tcase_add_test(tc_core, div_pos_numbers_1);
  tcase_add_test(tc_core, div_pos_numbers_2);
  tcase_add_test(tc_core, div_pos_numbers_3);
  tcase_add_test(tc_core, div_pos_numbers_4);
  tcase_add_test(tc_core, div_big_1);
  tcase_add_test(tc_core, div_big_2);
  tcase_add_test(tc_core, div_big_10);
  tcase_add_test(tc_core, div_neg_numbers);
  tcase_add_test(tc_core, div_big_numbers_overflow_1);
  tcase_add_test(tc_core, div_small_numbers_underflow_1);
  tcase_add_test(tc_core, div_small_numbers_underflow_2);
  tcase_add_test(tc_core, div_by_zero_1);
  tcase_add_test(tc_core, div_by_zero_2);
  tcase_add_test(tc_core, div_max_scale);
  tcase_add_test(tc_core, test_div_1);
  tcase_add_test(tc_core, div_max_min);
  tcase_add_test(tc_core, div_0_num);
  tcase_add_test(tc_core, div_0_0);
  tcase_add_test(tc_core, div_1_3);
  tcase_add_test(tc_core, div_2_3);
  tcase_add_test(tc_core, div_1_2);
  tcase_add_test(tc_core, div_scale5_to_scale2);
  tcase_add_test(tc_core, div_scale1_to_scale5);

  suite_add_tcase(s, tc_core);
  return s;
}
