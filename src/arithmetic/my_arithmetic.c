#include "../my_decimal.h"

int my_add(my_decimal value_1, my_decimal value_2, my_decimal *result) {
  int retVal = my_returnError(value_1, value_2, result);

  if (!retVal) {
    int scale_1 = my_getScale(value_1);
    int scale_2 = my_getScale(value_2);
    int sign_1 = my_getSign(value_1);
    int sign_2 = my_getSign(value_2);
    int diffScale = abs(scale_1 - scale_2);

    my_wideDecimal wideValue_1 = my_decimalToWideDecimal(value_1);
    my_wideDecimal wideValue_2 = my_decimalToWideDecimal(value_2);

    if (scale_1 < scale_2) {
      wideValue_1 = my_wideDecimalPowerTen(wideValue_1, diffScale);
    } else {
      wideValue_2 = my_wideDecimalPowerTen(wideValue_2, diffScale);
    }

    if (sign_1 == sign_2) {
      my_wideDecimal wideResult = {0};
      wideResult = my_simpleAddWideDecimal(wideValue_1, wideValue_2, NULL);
      int divCount = my_reductionToDecimal(&wideResult);
      int maxScale = max(scale_1, scale_2);
      retVal =
          my_createDecimal(wideResult, result, divCount, maxScale, sign_1);
    } else {
      value_2.bits[3] ^= (1u << 31);
      retVal = my_sub(value_1, value_2, result);
    }
  }

  return retVal;
}

int my_sub(my_decimal value_1, my_decimal value_2, my_decimal *result) {
  int retVal = my_returnError(value_1, value_2, result);

  if (!retVal) {
    int scale_1 = my_getScale(value_1);
    int scale_2 = my_getScale(value_2);
    int sign1 = my_getSign(value_1);
    int sign2 = my_getSign(value_2);

    if (sign1 == sign2) {
      my_wideDecimal wide_result = {0};

      int sign = my_returnSign(value_1, value_2, sign1, &wide_result);
      int divCount = my_reductionToDecimal(&wide_result);
      int maxScale = max(scale_1, scale_2);

      retVal = my_createDecimal(wide_result, result, divCount, maxScale, sign);
    } else {
      value_2.bits[3] ^= (1u << 31);
      retVal = my_add(value_1, value_2, result);
    }
  }

  return retVal;
}

int my_returnSign(my_decimal value_1, my_decimal value_2, int sign1,
                   my_wideDecimal *result) {
  int scale_1 = my_getScale(value_1);
  int scale_2 = my_getScale(value_2);
  int diffScale = abs(scale_1 - scale_2);

  my_wideDecimal wide_value_1 = my_decimalToWideDecimal(value_1);
  my_wideDecimal wide_value_2 = my_decimalToWideDecimal(value_2);

  if (scale_1 < scale_2) {
    wide_value_1 = my_wideDecimalPowerTen(wide_value_1, diffScale);
  } else {
    wide_value_2 = my_wideDecimalPowerTen(wide_value_2, diffScale);
  }

  int result_sign = -1;

  if (sign1) {
    *result =
        my_simpleSubWideDecimal(wide_value_2, wide_value_1, &result_sign);
  } else {
    *result =
        my_simpleSubWideDecimal(wide_value_1, wide_value_2, &result_sign);
  }

  return result_sign;
}

int my_mul(my_decimal value_1, my_decimal value_2, my_decimal *result) {
  int retVal = my_returnError(value_1, value_2, result);

  if (!retVal) {
    int scale_1 = my_getScale(value_1);
    int scale_2 = my_getScale(value_2);
    int sign_1 = my_getSign(value_1);
    int sign_2 = my_getSign(value_2);
    int diffScale = abs(scale_1 - scale_2);
    my_wideDecimal wideValue_1 = my_decimalToWideDecimal(value_1);
    my_wideDecimal wideValue_2 = my_decimalToWideDecimal(value_2);

    if (scale_1 < scale_2) {
      wideValue_1 = my_wideDecimalPowerTen(wideValue_1, diffScale);
    } else {
      wideValue_2 = my_wideDecimalPowerTen(wideValue_2, diffScale);
    }

    my_wideDecimal wideResult = {0};

    wideResult = my_simpleMulWideDecimal(wideValue_1, wideValue_2);
    wideResult = my_wideDecimalPowerTenDiv(wideResult, diffScale);
    int divCount = my_reductionToDecimal(&wideResult);
    retVal = my_createDecimal(wideResult, result, divCount, scale_1 + scale_2,
                               sign_1 ^ sign_2);
  }

  return retVal;
}

int my_div(my_decimal value_1, my_decimal value_2, my_decimal *result) {
  int retVal = my_returnError(value_1, value_2, result);

  if (retVal == 0 && (retVal = my_checkDivZero(value_2)) != 3) {
    int scale_1 = my_getScale(value_1);
    int scale_2 = my_getScale(value_2);
    int diffScale = abs(scale_1 - scale_2);
    int sign_1 = my_getSign(value_1);
    int sign_2 = my_getSign(value_2);

    my_wideDecimal wideValue_1 = my_decimalToWideDecimal(value_1);
    my_wideDecimal wideValue_2 = my_decimalToWideDecimal(value_2);

    if (scale_1 < scale_2) {
      wideValue_1 = my_wideDecimalPowerTen(wideValue_1, diffScale);
    } else {
      wideValue_2 = my_wideDecimalPowerTen(wideValue_2, diffScale);
    }

    my_wideDecimal remainder = {0};
    my_wideDecimal wideResInt =
        my_simpleDimple(wideValue_1, wideValue_2, &remainder);

    int finalScale = 0;
    if (my_check_zero_remain(remainder)) {
      finalScale = scale_1 - scale_2 + diffScale;
    } else {
      remainder = my_wideDecimalPowerTen(remainder, 28);
      remainder = my_simpleDimple(remainder, wideValue_2, NULL);

      wideResInt = my_wideDecimalPowerTen(wideResInt, 28);
      wideResInt = my_simpleAddWideDecimal(wideResInt, remainder, NULL);

      finalScale = scale_1 - scale_2 + 28 - diffScale;
    }

    int divCount = my_reductionToDecimal(&wideResInt);
    retVal = my_createDecimal(wideResInt, result, divCount, finalScale,
                               sign_1 ^ sign_2);
  }

  return retVal;
}