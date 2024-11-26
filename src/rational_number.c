#include "rational_number.h"

/* constant entities */
const rational_number rational_nan = { 0, 0 };
const rational_number rational_inf = { 1, 0 };
/* Essentially, NaN is encoded as 0 / 0, INF as a / 0 */

int is_rational_nan(rational_number q)
{
    return (q.nominator == 0 && q.denominator == 0);
}

int is_rational_inf(rational_number q)
{
    return (q.nominator != 0 && q.denominator == 0);
}

/* p == q ? */
int is_rational_equal(rational_number p, rational_number q)
{
    standartize_rational_number(&p);
    standartize_rational_number(&q);
    return (
        (p.nominator == q.nominator)
        && (p.denominator == q.denominator)
    );
}

void fprint_rational_number(FILE *stream, rational_number q)
{
    if (is_rational_nan(q)) {
        fprintf(stream, "nan");
        return;
    }
    if (is_rational_inf(q)) {
        fprintf(stream, "inf");
        return;
    }
    fprintf(stream, "%lld / %lld", q.nominator, q.denominator);
}

void print_rational_number(rational_number q)
{
    fprint_rational_number(stdout, q);
}

rational_number *new_rational_number(rn_int a, rn_int b)
{
    rational_number *q;
    q = (rational_number *) malloc(sizeof(*q));
    q->nominator = a;
    q->denominator = b;
    return q;
}

/* Greatest common divisor */
static rn_int gcd(rn_int a, rn_int b)
{
    while (a > 0 && b > 0) {
        if (a > b)
            a %= b;
        else
            b %= a;
    }
    if (a == 0)
        return b;
    return a;
}

static rn_int custom_abs(rn_int a)
{
    return (a >= 0) ? a : -a;
}
/*
static int min(int a, int b)
{
    return (a <= b ? a : b);
}

static int max(int a, int b)
{
    return (a >= b ? a : b);
}*/

/* Make rational number irreducible */
void standartize_rational_number(rational_number *q)
{
    rn_int a, b, s, d;

    if (is_rational_nan(*q)) {
        *q = rational_nan;
        return;
    }
    if (is_rational_inf(*q)) {
        *q = rational_inf;
        return;
    }

    s = 1;
    if (q->nominator * q->denominator < 0)
        s = -1;
    a = custom_abs(q->nominator);
    b = custom_abs(q->denominator);
    while ((d = gcd(a, b)) > 1) {
        a /= d;
        b /= d;
    }
    q->nominator = s*a;
    q->denominator = b;
}

/* Check a condition for n rational numbers;
If condition satisfied at a number, returns provided conditional number;
If not, returns NULL

Use-case: detect NaN */
static rational_number *const handle_any_rational(
    int (*check_condition)(rational_number),
    const rational_number *conditioned_value,
    int n,
    ...
)
{
    va_list valist;
    int i;
    rational_number w, *q;

    q = NULL;
    va_start(valist, n);
    for (i = 0; i < n; i++) {
        w = va_arg(valist, rational_number);
        if (check_condition(w)) {
            q = (rational_number *) conditioned_value;
            break;
        }
    }
    va_end(valist);
    return q;
}

/* p + q */
rational_number add_rationals(
    rational_number p, rational_number q
)
{
    rn_int a, b, c, d;
    rational_number w, *h;

    h = handle_any_rational(is_rational_nan, &rational_nan, 2, p, q);
    if (h != NULL)
        return *h;
    h = handle_any_rational(is_rational_inf, &rational_inf, 2, p, q);
    if (h != NULL)
        return *h;

    standartize_rational_number(&p);
    standartize_rational_number(&q);

    a = custom_abs(p.denominator);
    b = custom_abs(q.denominator);
    c = 1;
    while ((d = gcd(a, b)) > 1) {
        a /= d;
        b /= d;
        c *= d;
    }
    w.denominator = a*b*c;
    w.nominator = b*p.nominator + a*q.nominator;
    standartize_rational_number(&w);
    return w;
}

/* p - q = p + (-q) */
rational_number subtract_rationals(
    rational_number p, rational_number q
)
{
    q.nominator *= -1;
    return add_rationals(p, q);
}

/* p * q */
rational_number multiply_rationals(
    rational_number p, rational_number q
)
{
    rational_number w, *h;

    h = handle_any_rational(is_rational_nan, &rational_nan, 2, p, q);
    if (h != NULL)
        return *h;
    h = handle_any_rational(is_rational_inf, &rational_inf, 2, p, q);
    if (h != NULL)
        return *h;

    standartize_rational_number(&p);
    standartize_rational_number(&q);

    w.denominator = p.denominator*q.denominator;
    w.nominator = p.nominator*q.nominator;
    standartize_rational_number(&w);
    return w;
}
/* p = a/b --> b/a */
static rational_number invert_rational(rational_number q)
{
    rational_number w;
    
    /* if q is a rational_nan, then we swap zeros; */
    /* if q is a rational_inf, then the result is naturally zero */
    /* if q is a zero, standartization makes it to be a rational_inf */
    w.nominator = q.denominator;
    w.denominator = q.nominator;
    standartize_rational_number(&w);
    return w;
}
/* p / q = p * (1/q) */
rational_number divide_rationals(
    rational_number p, rational_number q
)
{
    rational_number iq, *h;
    int is_inf_q, is_inf_p;
    
    h = handle_any_rational(is_rational_nan, &rational_nan, 2, p, q);
    is_inf_p = is_rational_inf(p);
    is_inf_q = is_rational_inf(q);
    if (
        (p.nominator == 0 && q.nominator == 0)
        || (p.nominator == 0 && is_inf_q)
        || (is_inf_p && q.nominator == 0)
        || (is_inf_p && is_inf_q)
    )
        h = (rational_number *) &rational_nan;
    if (h != NULL)
        return *h;
    iq = invert_rational(q);
    return multiply_rationals(p, iq);
}

/* reduce(bivariate_rn_func, rn_array) */
rational_number reduce_rationals(
    bivariate_rn_func bvrnf, rational_number *rn_array, int n
)
{
    rational_number a;
    int i;

    a = rn_array[0];
    for (i = 1; i < n; i++) {
        a = bvrnf(a, rn_array[i]);
    }
    return a;
}

rational_number int_to_rational_number(rn_int a)
{
    rational_number rv;
    rv.nominator = a;
    rv.denominator = 1;
    return rv;
}
/* Author: Pavel Kharyuk, 2024 */