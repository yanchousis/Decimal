#include "../my_decimal.h"

my_wideDecimal my_decimalToWideDecimal(my_decimal number) {
  my_wideDecimal wideNumber = {0};

  for (int i = 0; i < 3; ++i) {
    wideNumber.bits[i] = (unsigned)number.bits[i];
  }

  return wideNumber;
}

my_decimal my_WideDecimalToDecimal(my_wideDecimal number) {
  my_decimal result = {0};

  for (int i = 0; i < 3; ++i) {
    result.bits[i] = (int)number.bits[i];
  }

  return result;
}

my_wideDecimal my_wideDecimalPowerTen(my_wideDecimal number, int power) {
  my_wideDecimal result = number;

  for (int i = 0; i < power; ++i) {
    my_wideDecimal valueShift3 = my_BinaryWideshiftL(result, 3);
    my_wideDecimal valueShift1 = my_BinaryWideshiftL(result, 1);

    result = my_simpleAddWideDecimal(valueShift3, valueShift1, NULL);
  }

  return result;
}

my_wideDecimal my_wideDecimalPowerTenDiv(my_wideDecimal number, int power) {
  for (int divCount = 0; divCount < power; ++divCount) {
    uint64_t remain = my_getRemain(&number);

    if (remain > 5) {
      my_wideDecimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};
      number = my_simpleAddWideDecimal(number, one, NULL);
    } else if (remain == 5) {
      if (my_isSetBit(number.bits[0], 0)) {
        my_wideDecimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};
        number = my_simpleAddWideDecimal(number, one, NULL);
      }
    }
  }

  return number;
}

uint64_t my_getRemain(my_wideDecimal *number) {
  uint64_t remain = 0;

  for (int i = 7; i >= 0; --i) {
    uint64_t temp = ((uint64_t)remain << 32) | number->bits[i];

    number->bits[i] = (unsigned int)(temp / 10);
    remain = temp % 10;
  }

  return remain;
}

my_wideDecimal my_simpleAddWideDecimal(my_wideDecimal value_1,
                                         my_wideDecimal value_2,
                                         int *overflow) {
  int carry = 0;
  my_wideDecimal result = {{0, 0, 0, 0, 0, 0, 0, 0}};

  for (int index = 0; index < 256; ++index) {
    int resultBit = my_isSetBit(value_1.bits[index / 32], index % 32) +
                    my_isSetBit(value_2.bits[index / 32], index % 32) + carry;

    carry = my_isSetBit(resultBit, 1);

    result.bits[index / 32] |= (unsigned)my_isSetBit(resultBit, 0)
                               << index % 32;
  }

  if (overflow) *overflow = carry;

  return result;
}

my_wideDecimal my_simpleSubWideDecimal(my_wideDecimal reduced,
                                         my_wideDecimal subtrahend,
                                         int *sign) {
  my_wideDecimal wide_result = {0};
  int overflow = 0;

  my_invert_wideDecimal(&subtrahend);
  wide_result = my_simpleAddWideDecimal(reduced, subtrahend, &overflow);

  if (overflow) {
    my_wideDecimal one_wideDecimal = {{1, 0, 0, 0, 0, 0, 0, 0}};

    wide_result =
        my_simpleAddWideDecimal(wide_result, one_wideDecimal, &overflow);

    if (sign) *sign = 0;
  } else {
    my_invert_wideDecimal(&wide_result);

    if (sign) *sign = 1;
  }

  return wide_result;
}

my_wideDecimal my_simpleMulWideDecimal(my_wideDecimal value_1,
                                         my_wideDecimal value_2) {
  my_wideDecimal wideResult = {0};

  for (int index = 0; index < 256; ++index) {
    if (my_isSetBit(value_1.bits[index / 32], index % 32)) {
      my_wideDecimal temp = my_BinaryWideshiftL(value_2, index);
      wideResult = my_simpleAddWideDecimal(wideResult, temp, NULL);
    }
  }

  return wideResult;
}

my_wideDecimal my_simpleDimple(my_wideDecimal value_1,
                                 my_wideDecimal value_2,
                                 my_wideDecimal *remainder) {
  my_wideDecimal wideResInt = {0};
  my_wideDecimal wideResRemain = {0};

  for (int i = 255; i >= 0; --i) {
    wideResRemain = my_BinaryWideshiftL(wideResRemain, 1);
    wideResRemain.bits[0] |= my_isSetBit(value_1.bits[i / 32], i % 32);

    if (my_greatEqualWide(wideResRemain, value_2)) {
      wideResRemain = my_simpleSubWideDecimal(wideResRemain, value_2, NULL);

      wideResInt.bits[i / 32] |= my_setBit(wideResInt.bits[i / 32], i % 32);
    }
  }

  if (remainder) *remainder = wideResRemain;

  return wideResInt;
}

my_wideDecimal my_BinaryWideshiftL(my_wideDecimal number, int shift) {
  for (int index = 255; index >= 0; --index) {
    if (index >= shift &&
        my_isSetBit(number.bits[(index - shift) / 32], (index - shift) % 32)) {
      number.bits[index / 32] = my_setBit(number.bits[index / 32], index % 32);
    } else {
      number.bits[index / 32] =
          my_resetBit(number.bits[index / 32], index % 32);
    }
  }

  return number;
}

int my_reductionToDecimal(my_wideDecimal *number) {
  int divCount = 0;

  for (int i = 7; i > 2; --i) {
    while (number->bits[i]) {
      *number = my_wideDecimalPowerTenDiv(*number, 1);
      ++divCount;
    }
  }

  return divCount;
}

void my_invert_wideDecimal(my_wideDecimal *value) {
  for (int i = 0; i < 8; i++) {
    value->bits[i] = ~(value->bits[i]);
  }
}

int my_greatEqualWide(my_wideDecimal value_1, my_wideDecimal value_2) {
  int retVal = 1;

  for (int i = 7; i >= 0; --i) {
    unsigned num1 = value_1.bits[i];
    unsigned num2 = value_2.bits[i];

    if (num1 > num2) {
      break;
    } else if (num1 < num2) {
      retVal = 0;
      break;
    }
  }

  return retVal;
}

int my_check_zero_remain(my_wideDecimal value) {
  int k = 0;

  for (int i = 0; i < 7; ++i) {
    if (value.bits[i] == 0) k++;
  }

  return (k == 7) ? 1 : 0;
}