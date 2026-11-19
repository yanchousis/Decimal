#include "../my_decimal.h"

int my_getSign(my_decimal number) { return my_isSetBit(number.bits[3], 31); }

int my_getScale(my_decimal number) { return (char)(number.bits[3] >>= 16); }

void my_initDecLMHScSign(my_decimal *number, int n0, int n1, int n2,
                          int scale, int sign) {
  if (number) {
    number->bits[0] = n0;
    number->bits[1] = n1;
    number->bits[2] = n2;

    my_fillScaleSign(number, scale, sign);
  }
}

void my_fillScaleSign(my_decimal *number, int scale, int sign) {
  sign = (unsigned)sign << 31;
  scale = (unsigned)scale << 16;
  number->bits[3] = scale + sign;
}

int my_createDecimal(my_wideDecimal src, my_decimal *result, int divCount,
                      int scale, int sign) {
  int retVal = OK;

  if (divCount > scale) {
    retVal = (!sign) ? BIGNUMBER : LOWNUMBER;

    reset(result);
  } else {
    *result = my_WideDecimalToDecimal(src);
    *result = my_removeTrailingZeroPoint(*result, &scale);
    my_fillScaleSign(result, scale - divCount, sign);
    my_negZeroToPos(result);
  }

  return retVal;
}

void my_negZeroToPos(my_decimal *result) {
  int k = 0;

  for (int i = 0; i < 3; ++i) {
    if (result->bits[i] == 0) ++k;
  }

  if (k == 3) result->bits[3] = my_resetBit(result->bits[3], 31);
}

int my_returnError(my_decimal value_1, my_decimal value_2,
                    my_decimal *result) {
  int error = isCorrectDecimal(value_1);
  if (error) error = isCorrectDecimal(value_2);

  if (result) reset(result);

  int retVal = !error;

  int scale_1 = my_getScale(value_1);
  int scale_2 = my_getScale(value_2);

  if (scale_1 < 0 || scale_2 < 0) {
    retVal = BIGNUMBER;
  } else if (scale_1 > 28 || scale_2 > 28) {
    retVal = LOWNUMBER;
  }

  return retVal;
}

int my_checkDivZero(my_decimal number) {
  int retVal = OK;
  char k = 0;

  for (int i = 0; i < 3; ++i) {
    if (number.bits[i] == 0) {
      ++k;
    }
  }

  if (k == 3) retVal = DIVBYZERO;

  return retVal;
}

my_decimal my_removeTrailingZeroPoint(my_decimal number, int *scale) {
  my_wideDecimal temp = my_decimalToWideDecimal(number);

  while (!my_getRemain(&temp) && *scale > 0) {
    number = my_WideDecimalToDecimal(temp);
    *scale -= 1;
  }

  return number;
}

my_decimal my_absDecimal(my_decimal number) {
  number.bits[3] = my_resetBit(number.bits[3], 31);

  return number;
}

void my_copy(my_decimal value, my_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
}

void reset(my_decimal *num) {
  num->bits[0] = 0;
  num->bits[1] = 0;
  num->bits[2] = 0;
  num->bits[3] = 0;
}

int my_isSetBit(int number, int index) {
  return (number & (1u << index)) != 0;
}

int my_setBit(int number, int index) { return (number | (1u << index)); }

int my_resetBit(int number, int index) { return (number & ~(1u << index)); }

int max(const int a, const int b) { return (a > b) ? a : b; }