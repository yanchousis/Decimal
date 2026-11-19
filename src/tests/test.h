#ifndef TEST_H_
#define TEST_H_

#include <check.h>

#include "../my_decimal.h"

Suite *test_is_less(void);
Suite *test_is_less_or_equal(void);
Suite *test_is_greater(void);
Suite *test_is_greater_or_equal(void);
Suite *test_is_equal(void);
Suite *test_is_not_equal(void);
Suite *test_add(void);
Suite *test_div(void);
Suite *test_mul(void);
Suite *test_sub(void);
Suite *test_other(void);
Suite *test_converters(void);

#endif
