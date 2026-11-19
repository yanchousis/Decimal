#include "../my_decimal.h"

my_decimal decimal_ten_pow(int exp) {
  const my_decimal all_ten_pows[29] = {
      [0] = {{0x1, 0x0, 0x0, 0x0}},
      [1] = {{0xA, 0x0, 0x0, 0x0}},
      [2] = {{0x64, 0x0, 0x0, 0x0}},
      [3] = {{0x3E8, 0x0, 0x0, 0x0}},
      [4] = {{0x2710, 0x0, 0x0, 0x0}},
      [5] = {{0x186A0, 0x0, 0x0, 0x0}},
      [6] = {{0xF4240, 0x0, 0x0, 0x0}},
      [7] = {{0x989680, 0x0, 0x0, 0x0}},
      [8] = {{0x5F5E100, 0x0, 0x0, 0x0}},
      [9] = {{0x3B9ACA00, 0x0, 0x0, 0x0}},
      [10] = {{0x540BE400, 0x2, 0x0, 0x0}},
      [11] = {{0x4876E800, 0x17, 0x0, 0x0}},
      [12] = {{0xD4A51000, 0xE8, 0x0, 0x0}},
      [13] = {{0x4E72A000, 0x918, 0x0, 0x0}},
      [14] = {{0x107A4000, 0x5AF3, 0x0, 0x0}},
      [15] = {{0xA4C68000, 0x38D7E, 0x0, 0x0}},
      [16] = {{0x6FC10000, 0x2386F2, 0x0, 0x0}},
      [17] = {{0x5D8A0000, 0x1634578, 0x0, 0x0}},
      [18] = {{0xA7640000, 0xDE0B6B3, 0x0, 0x0}},
      [19] = {{0x89E80000, 0x8AC72304, 0x0, 0x0}},
      [20] = {{0x63100000, 0x6BC75E2D, 0x5, 0x0}},
      [21] = {{0xDEA00000, 0x35C9ADC5, 0x36, 0x0}},
      [22] = {{0xB2400000, 0x19E0C9BA, 0x21E, 0x0}},
      [23] = {{0xF6800000, 0x2C7E14A, 0x152D, 0x0}},
      [24] = {{0xA1000000, 0x1BCECCED, 0xD3C2, 0x0}},
      [25] = {{0x4A000000, 0x16140148, 0x84595, 0x0}},
      [26] = {{0xE4000000, 0xDCC80CD2, 0x52B7D2, 0x0}},
      [27] = {{0xE8000000, 0x9FD0803C, 0x33B2E3C, 0x0}},
      [28] = {{0x10000000, 0x3E250261, 0x204FCE5E, 0x0}}};

  return all_ten_pows[exp];
};

int my_negate(my_decimal value, my_decimal *result) {
  int flag = 0;

  if (isCorrectDecimal(value) && result) {
    my_copy(value, result);

    if (my_isSetBit(value.bits[3], 31) == 1) {
      result->bits[3] = my_resetBit(result->bits[3], 31);
    } else if (my_isSetBit(value.bits[3], 31) == 0) {
      result->bits[3] = my_setBit(result->bits[3], 31);
    }
  } else {
    flag = 1;
  }

  return flag;
}

my_decimal my_decimal_shift_left(my_decimal value, int index) {
  while (index) {
    value.bits[2] = value.bits[2] << 1;

    if (my_isSetBit(value.bits[1], 31)) {
      value.bits[2] = my_setBit(value.bits[2], 0);
    }

    value.bits[1] = value.bits[1] << 1;

    if (my_isSetBit(value.bits[0], 31)) {
      value.bits[1] = my_setBit(value.bits[1], 0);
    }

    value.bits[0] = value.bits[0] << 1;
    index--;
  }

  return value;
}
my_decimal my_decimal_shift_right(my_decimal value, int index) {
  while (index) {
    value.bits[0] = value.bits[0] >> 1;

    if (my_isSetBit(value.bits[1], 0)) {
      value.bits[0] = my_setBit(value.bits[0], 31);
    }

    value.bits[1] = value.bits[1] >> 1;

    if (my_isSetBit(value.bits[2], 0)) {
      value.bits[1] = my_setBit(value.bits[1], 31);
    }

    value.bits[2] = value.bits[2] >> 1;

    index--;
  }

  return value;
}

int my_get_fraction_part(my_decimal value, int exp) {
  int flag = 0;
  my_decimal tenPow = decimal_ten_pow(exp);
  value.bits[3] = 0;
  my_decimal rest = my_decimal_division(value, tenPow, &value);
  tenPow = my_decimal_shift_right(tenPow, 1);
  if (my_is_greater_or_equal(rest, tenPow)) {
    flag = 1;
  }
  return flag;
}

my_decimal my_decimal_division(my_decimal value, my_decimal tenPow,
                                 my_decimal *result) {
  my_decimal one = {{1, 0, 0, 0}};
  my_decimal intOfDiv = tenPow;
  my_decimal tmp = tenPow;
  my_decimal tmpValue = value;

  while (my_is_greater_or_equal(tmpValue, tenPow)) {
    int i = 0;

    while (my_is_less_or_equal(tmp, value) &&
           my_isSetBit(tmp.bits[2], 31) == 0) {
      tmp = my_decimal_shift_left(tmp, 1);

      if (my_is_less_or_equal(tmp, value)) {
        my_copy(tmp, &intOfDiv);
        i++;
      }
    }

    my_add(*result, my_decimal_shift_left(one, i), result);
    my_sub(tmpValue, intOfDiv, &tmpValue);

    if (my_is_greater_or_equal(tmpValue, tenPow)) {
      my_copy(tmpValue, &value);
    }

    intOfDiv = tenPow;
    tmp = tenPow;
  }
  return tmpValue;
}

int my_truncate(my_decimal value, my_decimal *result) {
  int res = 0;

  if (isCorrectDecimal(value) && result) {
    reset(result);
    int exp = my_getScale(value);

    if (!exp) {
      my_copy(value, result);
    } else {
      my_decimal tenPow = decimal_ten_pow(exp);
      int sign = my_getSign(value);

      value.bits[3] = 0;

      my_decimal_division(value, tenPow, result);

      if (sign) result->bits[3] = my_setBit(result->bits[3], 31);
    }
  } else {
    res = 1;
  }

  return res;
}

int my_floor(my_decimal value, my_decimal *result) {
  int res = 0;

  if (isCorrectDecimal(value) && result) {
    int exp = my_getScale(value);

    if (!exp) {
      my_copy(value, result);
    } else {
      my_truncate(value, result);

      if (my_getSign(*result)) {
        my_decimal one = {{1, 0, 0, 0}};
        my_sub(*result, one, result);
      }
    }
  } else {
    res = 1;
  }

  return res;
}

int my_round(my_decimal value, my_decimal *result) {
  int retVal = 0;

  if (isCorrectDecimal(value) && result) {
    int scale = my_getScale(value);
    int sign = my_getSign(value);

    if (scale > 0) {
      int fractionalPart = my_get_fraction_part(value, scale);

      my_truncate(value, result);

      if (fractionalPart) {
        my_decimal one = {{1, 0, 0, 0}};
        my_decimal absResult = my_absDecimal(*result);
        my_add(absResult, one, result);

        if (sign == 1) my_negate(*result, result);
      }
    } else {
      my_copy(value, result);
    }
  } else {
    retVal = 1;
  }

  return retVal;
}