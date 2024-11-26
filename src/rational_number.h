/* C implementation of rational numbers
with structs */

#ifndef __RATIONAL_NUMBER_H__
#define __RATIONAL_NUMBER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef long long int rn_int;

struct rational_number_st {
    rn_int nominator;
    rn_int denominator;
};
typedef struct rational_number_st rational_number;

/* NaN and Inf */
extern const rational_number rational_nan;
extern const rational_number rational_inf;

typedef rational_number (*univariate_rn_func)(rational_number);
typedef rational_number (*bivariate_rn_func)(rational_number, rational_number);

int is_rational_nan(rational_number q);
int is_rational_inf(rational_number q);
int is_rational_equal(rational_number p, rational_number q);
void fprint_rational_number(FILE *stream, rational_number q);
void print_rational_number(rational_number q);
rational_number *new_rational_number(rn_int a, rn_int b);

/* Irreducible fraction, sign is moved into nominator */
void standartize_rational_number(rational_number *q);

/* Arithmetics */
rational_number add_rationals(
    rational_number p, rational_number q
);
rational_number subtract_rationals(
    rational_number p, rational_number q
);
rational_number multiply_rationals(
    rational_number p, rational_number q
);
rational_number divide_rationals(
    rational_number p, rational_number q
);

/* Vectorization */
rational_number reduce_rationals(
    bivariate_rn_func bvrnf, rational_number *rn_array, int n
);

rational_number int_to_rational_number(rn_int a);

#endif
/* Author: Pavel Kharyuk, 2024 */