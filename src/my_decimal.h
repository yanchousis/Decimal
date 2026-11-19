#ifndef MY_DECIMAL_H_
#define MY_DECIMAL_H_

#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum errors {
  OK = 0,
  BIGNUMBER = 1,
  LOWNUMBER = 2,
  DIVBYZERO = 3,
};

typedef struct {
  int bits[4];
} my_decimal;

typedef struct {
  unsigned int bits[8];
} my_wideDecimal;

int my_add(my_decimal, my_decimal, my_decimal *);
int my_sub(my_decimal, my_decimal, my_decimal *);
int my_mul(my_decimal, my_decimal, my_decimal *);
int my_div(my_decimal, my_decimal, my_decimal *);

int my_returnSign(my_decimal, my_decimal, int, my_wideDecimal *);
int my_returnError(my_decimal, my_decimal, my_decimal *);
int my_checkDivZero(my_decimal);

int my_is_less(my_decimal, my_decimal);
int my_is_less_or_equal(my_decimal, my_decimal);
int my_is_greater(my_decimal, my_decimal);
int my_is_greater_or_equal(my_decimal, my_decimal);
int my_is_equal(my_decimal, my_decimal);
int my_is_not_equal(my_decimal, my_decimal);

void casting_zero(my_decimal *, my_decimal *);
void exp_leveling(my_wideDecimal *, my_wideDecimal *, my_decimal,
                  my_decimal);

int my_from_int_to_decimal(int, my_decimal *);
int my_from_float_to_decimal(float, my_decimal *);
int my_from_decimal_to_int(my_decimal, int *);
int my_from_decimal_to_float(my_decimal, float *);

int my_negate(my_decimal, my_decimal *);
int my_truncate(my_decimal, my_decimal *);
int my_floor(my_decimal, my_decimal *);
int my_round(my_decimal, my_decimal *);

my_decimal decimal_ten_pow(int);
my_decimal my_decimal_shift_left(my_decimal, int);
my_decimal my_decimal_shift_right(my_decimal, int);
int my_get_fraction_part(my_decimal, int);
my_decimal my_decimal_division(my_decimal, my_decimal, my_decimal *);

my_wideDecimal my_decimalToWideDecimal(my_decimal);
my_decimal my_WideDecimalToDecimal(my_wideDecimal);
int my_createDecimal(my_wideDecimal, my_decimal *, int, int, int);

int my_isSetBit(int, int);
int my_setBit(int, int);
int my_resetBit(int, int);
int max(const int, const int);

int my_getSign(my_decimal);
int my_getScale(my_decimal);
void my_initDecLMHScSign(my_decimal *, int, int, int, int, int);
void my_fillScaleSign(my_decimal *, int, int);
my_decimal my_simple_dimple(my_decimal, my_decimal, my_decimal *);
my_decimal my_absDecimal(my_decimal);
my_decimal my_removeTrailingZeroPoint(my_decimal, int *);
void my_negZeroToPos(my_decimal *);
void my_copy(my_decimal, my_decimal *);

void reset(my_decimal *);
int isCorrectDecimal(my_decimal);
int my_fillFloatDecimal(double, my_decimal *, int, int);
int my_isInfOrNan(my_decimal);

my_wideDecimal my_BinaryWideshiftL(my_wideDecimal, int);
my_wideDecimal my_simpleAddWideDecimal(my_wideDecimal, my_wideDecimal,
                                         int *);
my_wideDecimal my_simpleMulWideDecimal(my_wideDecimal, my_wideDecimal);
my_wideDecimal my_simpleSubWideDecimal(my_wideDecimal, my_wideDecimal,
                                         int *);
my_wideDecimal my_simpleDimple(my_wideDecimal, my_wideDecimal,
                                 my_wideDecimal *);
my_wideDecimal my_wideDecimalPowerTen(my_wideDecimal, int);
my_wideDecimal my_wideDecimalPowerTenDiv(my_wideDecimal, int);
uint64_t my_getRemain(my_wideDecimal *);
int my_reductionToDecimal(my_wideDecimal *);
void my_invert_wideDecimal(my_wideDecimal *);
int my_greatEqualWide(my_wideDecimal, my_wideDecimal);
int my_check_zero_remain(my_wideDecimal);

#endif