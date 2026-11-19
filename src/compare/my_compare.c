#include "../my_decimal.h"

void casting_zero(my_decimal* num1, my_decimal* num2) {
  if (num1->bits[0] == 0 && num1->bits[1] == 0 && num1->bits[2] == 0) {
    *num1 = my_absDecimal(*num1);
  }
  if (num2->bits[0] == 0 && num2->bits[1] == 0 && num2->bits[2] == 0) {
    *num2 = my_absDecimal(*num2);
  }
}

void exp_leveling(my_wideDecimal* wideNum1, my_wideDecimal* wideNum2,
                  my_decimal num1, my_decimal num2) {
  int exp1 = (char)(num1.bits[3] >> 16);
  int exp2 = (char)(num2.bits[3] >> 16);

  if (exp1 > 28) {
    *wideNum1 = my_wideDecimalPowerTenDiv(*wideNum1, exp1 - 28);
    exp1 = 28;
  }
  if (exp2 > 28) {
    *wideNum2 = my_wideDecimalPowerTenDiv(*wideNum2, exp2 - 28);
    exp2 = 28;
  }
  int exp_add = exp1 - exp2;
  if (exp_add > 0) {
    *wideNum2 = my_wideDecimalPowerTen(*wideNum2, exp_add);
  } else if (exp_add < 0) {
    exp_add = abs(exp_add);
    *wideNum1 = my_wideDecimalPowerTen(*wideNum1, exp_add);
  }
}

int my_is_less(my_decimal num1, my_decimal num2) {
  casting_zero(&num1, &num2);

  int res = 0;
  int sign1 = my_getSign(num1);
  int sign2 = my_getSign(num2);

  my_wideDecimal wideNum1 = my_decimalToWideDecimal(num1);
  my_wideDecimal wideNum2 = my_decimalToWideDecimal(num2);

  if (sign1 == sign2) {
    exp_leveling(&wideNum1, &wideNum2, num1, num2);
    int match = 0;
    int out = 0;
    for (int i = 6; i >= 0 && !out; i--) {
      for (int j = 31; j >= 0 && !out; j--) {
        if (!my_isSetBit(wideNum1.bits[i], j) &&
            my_isSetBit(wideNum2.bits[i], j)) {
          res = 1;
        } else if (my_isSetBit(wideNum1.bits[i], j) &&
                   !my_isSetBit(wideNum2.bits[i], j)) {
          out = 1;
        } else
          match++;
      }
    }
    if (match != 7 * 32 && sign1) res = (res + 1) % 2;
  } else {
    res = (sign1 > sign2);
  }
  return res;
}

int my_is_less_or_equal(my_decimal num1, my_decimal num2) {
  casting_zero(&num1, &num2);

  int res = 0;
  int sign1 = my_getSign(num1);
  int sign2 = my_getSign(num2);

  my_wideDecimal wideNum1 = my_decimalToWideDecimal(num1);
  my_wideDecimal wideNum2 = my_decimalToWideDecimal(num2);

  if (sign1 == sign2) {
    exp_leveling(&wideNum1, &wideNum2, num1, num2);
    int match = 0;
    int out = 0;
    for (int i = 6; i >= 0 && !out; i--) {
      for (int j = 31; j >= 0 && !out; j--) {
        if (!my_isSetBit(wideNum1.bits[i], j) &&
            my_isSetBit(wideNum2.bits[i], j)) {
          res = 1;
        } else if (my_isSetBit(wideNum1.bits[i], j) &&
                   !my_isSetBit(wideNum2.bits[i], j)) {
          out = 1;
        } else
          match++;
      }
    }
    if (sign1) res = (res + 1) % 2;
    if (!res) res = (match == 7 * 32);
  } else {
    res = (sign1 > sign2);
  }

  return res;
}

int my_is_greater(my_decimal num1, my_decimal num2) {
  casting_zero(&num1, &num2);

  int res = 0;
  int sign1 = my_getSign(num1);
  int sign2 = my_getSign(num2);

  my_wideDecimal wideNum1 = my_decimalToWideDecimal(num1);
  my_wideDecimal wideNum2 = my_decimalToWideDecimal(num2);

  if (sign1 == sign2) {
    exp_leveling(&wideNum1, &wideNum2, num1, num2);
    int match = 0;
    int out = 0;
    for (int i = 6; i >= 0 && !out; i--) {
      for (int j = 31; j >= 0 && !out; j--) {
        if (my_isSetBit(wideNum1.bits[i], j) &&
            !my_isSetBit(wideNum2.bits[i], j)) {
          res = 1;
        } else if (!my_isSetBit(wideNum1.bits[i], j) &&
                   my_isSetBit(wideNum2.bits[i], j)) {
          out = 1;
        } else
          match++;
      }
    }
    if (match != 7 * 32 && sign1) res = (res + 1) % 2;
  } else {
    res = (sign1 < sign2);
  }
  return res;
}

int my_is_greater_or_equal(my_decimal num1, my_decimal num2) {
  casting_zero(&num1, &num2);

  int res = 0;
  int sign1 = my_getSign(num1);
  int sign2 = my_getSign(num2);
  my_wideDecimal wideNum1 = my_decimalToWideDecimal(num1);
  my_wideDecimal wideNum2 = my_decimalToWideDecimal(num2);

  if (sign1 == sign2) {
    exp_leveling(&wideNum1, &wideNum2, num1, num2);

    int match = 0;
    int out = 0;
    for (int i = 6; i >= 0 && !out; i--) {
      for (int j = 31; j >= 0 && !out; j--) {
        if (my_isSetBit(wideNum1.bits[i], j) &&
            !my_isSetBit(wideNum2.bits[i], j)) {
          res = 1;
        } else if (!my_isSetBit(wideNum1.bits[i], j) &&
                   my_isSetBit(wideNum2.bits[i], j)) {
          out = 1;
        } else
          match++;
      }
    }
    if (sign1) res = (res + 1) % 2;
    if (!res) res = (match == 7 * 32);
  } else {
    res = (sign1 < sign2);
  }

  return res;
}

int my_is_equal(my_decimal num1, my_decimal num2) {
  casting_zero(&num1, &num2);

  int res = 1;
  int sign1 = my_getSign(num1);
  int sign2 = my_getSign(num2);

  my_wideDecimal wideNum1 = my_decimalToWideDecimal(num1);
  my_wideDecimal wideNum2 = my_decimalToWideDecimal(num2);

  if (sign1 == sign2) {
    exp_leveling(&wideNum1, &wideNum2, num1, num2);
    for (int i = 6; i >= 0 && res; i--) {
      for (int j = 31; j >= 0 && res; j--) {
        if (my_isSetBit(wideNum1.bits[i], j) !=
            my_isSetBit(wideNum2.bits[i], j)) {
          res = 0;
        }
      }
    }
  } else {
    res = 0;
  }
  return res;
}

int my_is_not_equal(my_decimal num1, my_decimal num2) {
  casting_zero(&num1, &num2);

  int res = 0;
  int sign1 = my_getSign(num1);
  int sign2 = my_getSign(num2);

  my_wideDecimal wideNum1 = my_decimalToWideDecimal(num1);
  my_wideDecimal wideNum2 = my_decimalToWideDecimal(num2);

  if (sign1 == sign2) {
    exp_leveling(&wideNum1, &wideNum2, num1, num2);
    for (int i = 6; i >= 0 && !res; i--) {
      for (int j = 31; j >= 0 && !res; j--) {
        if (my_isSetBit(wideNum1.bits[i], j) !=
            my_isSetBit(wideNum2.bits[i], j)) {
          res = 1;
        }
      }
    }
  } else {
    res = 1;
  }
  return res;
}