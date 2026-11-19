#include "../my_decimal.h"

#define POW32 pow(2, 32)

int my_from_int_to_decimal(int src, my_decimal *dst) {
  int res = 0;
  if (dst) {
    reset(dst);

    if (src < 0) {
      dst->bits[3] = my_setBit(dst->bits[3], 31);
      src = abs(src);
    }
    dst->bits[0] = src;
  } else {
    res = 1;
  }
  return res;
}

int my_from_float_to_decimal(float src, my_decimal *dst) {
  int res = 0;
  double src_dbl = (double)src;
  if (fabs(src_dbl) < 1e-28) res = 1;
  if (!res && dst) {
    reset(dst);
    int sign = 0;
    if (src_dbl < 0) {
      sign = 1;
      src_dbl = fabs(src_dbl);
    }
    int exp = 0;
    int count_signific = 0;
    double temp = src_dbl;
    for (int i = 1; temp / i >= 1; i *= 10) {
      count_signific++;
      if (i == 1000000) {
        temp /= 1000000;
        i = 1;
      }
      temp -= fmod(temp, 1);
    }
    if (count_signific > 7) {
      int over = count_signific - 7;
      double remainder = fmod(src_dbl, pow(10, over));
      if (remainder >= 5 * pow(10, over - 1)) {
        src_dbl += (pow(10, over) - remainder);
      } else {
        src_dbl -= remainder;
      }
    }
    while (count_signific < 7 && exp < 28) {
      src_dbl *= 10;
      if (count_signific || (int)fmod(src_dbl, 10)) count_signific++;
      exp++;
    }
    if ((int)fmod(src_dbl * 10, 10) >= 5) {
      src_dbl += 1;
    }
    src_dbl -= fmod(src_dbl, 1);
    res = my_fillFloatDecimal(src_dbl, dst, exp, sign);
    *dst = my_removeTrailingZeroPoint(*dst, &exp);
    dst->bits[3] |= (exp << 16) + ((unsigned)sign << 31);
  } else {
    res = 1;
  }
  return res;
}

int my_from_decimal_to_int(my_decimal src, int *dst) {
  int res = 0;

  if (isCorrectDecimal(src) && dst) {
    int sign = my_getSign(src) ? -1 : 1;
    int exp = my_getScale(src);
    *dst = ((uint32_t)src.bits[2] * POW32 * POW32 +
            (uint32_t)src.bits[1] * POW32 + (uint32_t)src.bits[0]) /
           pow(10, exp) * sign;
  } else {
    res = 1;
  }

  return res;
}

int my_from_decimal_to_float(my_decimal src, float *dst) {
  int res = 0;

  if (isCorrectDecimal(src) && dst) {
    int sign = my_getSign(src) ? -1 : 1;
    int exp = my_getScale(src);
    *dst = (double)((uint32_t)src.bits[2] * POW32 * POW32 +
                    (uint32_t)src.bits[1] * POW32 + (uint32_t)src.bits[0]) /
           pow(10, exp) * sign;
  } else {
    res = 1;
  }

  return res;
}

int isCorrectDecimal(my_decimal src) {
  int res = 1;

  if (src.bits[3] & 0xFFFF) {
    res = 0;
  } else if (my_getScale(src) > 28) {
    res = 0;
  } else if (((src.bits[3] >> 24) & 0b1111111) > 0) {
    res = 0;
  } else if (my_isInfOrNan(src)) {
    res = 0;
  }

  return res;
}

int my_isInfOrNan(my_decimal src) {
  int res = 0;

  if ((!src.bits[0] && !src.bits[1] && !src.bits[2] &&
       src.bits[3] == 0xFF0000) ||
      (!src.bits[0] && !src.bits[1] && !src.bits[2] &&
       (unsigned)src.bits[3] == 0xFF0000 + (1u << 31)) ||
      (src.bits[0] == -1 && src.bits[1] == -1 && src.bits[2] == -1 &&
       src.bits[3] == 0xFF0000)) {
    res = 1;
  }

  return res;
}

int my_fillFloatDecimal(double src, my_decimal *dst, int exp, int sign) {
  int res = 0;
  unsigned int over = src / POW32 / POW32 / POW32;

  if (over != 0) {
    res = 1;
  } else {
    unsigned int low = fmod(src, POW32);
    unsigned int middle = fmod(src / POW32, POW32);
    unsigned int high = src / POW32 / POW32;
    my_initDecLMHScSign(dst, low, middle, high, exp, sign);
  }

  return res;
}