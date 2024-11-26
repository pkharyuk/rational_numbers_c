#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "rational_number.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((*a)))

static void print_incorrect_example_is_rational_equal(
    rational_number a, rational_number b,
    int result, int expected_result
)
{
    fprintf(stderr, "Incorrect: is_rational_equal(");
    fprint_rational_number(stderr, a);
    fprintf(stderr, ", ");
    fprint_rational_number(stderr, b);
    fprintf(stderr, ") = %d", result);
    fprintf(stderr, " [expected: %d]\n", expected_result);
}

/* if b is a null pointer, it behaves as in the univariate case */
static void print_incorrect_example_rnfrnrn(
    const rational_number *a,
    const rational_number *b,
    const rational_number *result,
    const rational_number *expected_result,
    const char *func_name
)
{
    fprintf(stderr, "Incorrect: %s(", func_name);
    fprint_rational_number(stderr, *a);
    if (b != NULL) {
        fprintf(stderr, ", ");
        fprint_rational_number(stderr, *b);
    }
    fprintf(stderr, ") = ");
    fprint_rational_number(stderr, *result);
    fprintf(stderr, " [expected: ");
    fprint_rational_number(stderr, *expected_result);
    fprintf(stderr, "]\n");
}

static int test_is_rational_equal()
{
    const rational_number input_a[] = {
        {1, 7}, {9, -3}, {4, 5}, {0, 7}, {9, 0}, {0, 78}
    };
    const rational_number input_b[] = {
        {1, 7}, {-3, 1}, {-4, 5}, {0, -7}, {1, 0}, {0, 5}
    };
    const int expected_output[] = {
        1, 1, 0, 1, 1, 1
    };

    int i, n, c_output;

    n = ARRAY_SIZE(input_a);
    assert(n == ARRAY_SIZE(input_b));
    assert(n == ARRAY_SIZE(expected_output));
    for (i = 0; i < n; i++) {
        c_output = is_rational_equal(input_a[i], input_b[i]);
        if (c_output != expected_output[i]) {
            print_incorrect_example_is_rational_equal(
                input_a[i], input_b[i], c_output, expected_output[i]
            );
            return 1;
        }
        c_output = is_rational_equal(input_b[i], input_a[i]);
        if (c_output != expected_output[i]) {
            print_incorrect_example_is_rational_equal(
                input_b[i], input_a[i], c_output, expected_output[i]
            );
            return 1;
        }
    }
    return 0;
}

static void print_incorrect_example_ifrn(
    rational_number a, int result, int expected_result,
    const char *func_name
)
{
    fprintf(stderr, "Incorrect: %s(", func_name);
    fprint_rational_number(stderr, a);
    fprintf(stderr, ") = %d", result);
    fprintf(stderr, " [expected: %d]\n", expected_result);
}

static int test_unary_int_function(
    int (*ufunc)(rational_number),
    const char *func_name,
    const rational_number *input_a,
    const int *expected_output,
    int n
)
{
    int i, c_output;

    for (i = 0; i < n; i++) {
        c_output = ufunc(input_a[i]);
        if (c_output != expected_output[i]) {
            print_incorrect_example_ifrn(
                input_a[i], c_output, expected_output[i], func_name
            );
            return 1;
        }
    }
    return 0;
}

static int test_unary_rn_function(
    rational_number (*ufunc)(rational_number),
    const char *func_name,
    const rational_number *input_a,
    const rational_number *expected_output,
    int n
)
{
    int i, c_result;
    rational_number c_output;

    for (i = 0; i < n; i++) {
        c_output = ufunc(input_a[i]);
        c_result = is_rational_equal(c_output, expected_output[i]);
        if (c_result == 0) {
            print_incorrect_example_rnfrnrn(
                &(input_a[i]),
                NULL,
                &(c_output),
                &(expected_output[i]),
                func_name
            );
            return 1;
        }
    }
    return 0;
}

static int test_binary_rn_function(
    rational_number (*bfunc)(rational_number, rational_number),
    const char *func_name,
    const rational_number *input_a,
    const rational_number *input_b,
    const rational_number *expected_output,
    int n
)
{
    int i, c_result;
    rational_number c_output;

    for (i = 0; i < n; i++) {
        c_output = bfunc(input_a[i], input_b[i]);
        c_result = is_rational_equal(c_output, expected_output[i]);
        if (c_result == 0) {
            print_incorrect_example_rnfrnrn(
                &input_a[i],
                &input_b[i],
                &c_output,
                &expected_output[i],
                func_name
            );
            return 1;
        }
    }
    return 0;
}

static int test_is_rational_nan()
{
    const rational_number rn_array[] = {
        {0, 0}, {1, 0}, {4, 5}, {0, 7}
    };
    const int expected_result[] = {
        1, 0, 0, 0
    };

    int n, result;

    n = ARRAY_SIZE(rn_array);
    assert(n == ARRAY_SIZE(expected_result));
    result = test_unary_int_function(
        is_rational_nan,
        "is_rational_nan",
        rn_array,
        expected_result,
        n
    );
    return result;
}

static int test_is_rational_inf()
{
    const rational_number rn_array[] = {
        {0, 0}, {1, 0}, {4, 5}, {0, 7}, {-4, 0}, {-4, 1}
    };
    const int expected_result[] = {
        0, 1, 0, 0, 1, 0
    };

    int n, result;

    n = ARRAY_SIZE(rn_array);
    assert(n == ARRAY_SIZE(expected_result));
    result = test_unary_int_function(
        is_rational_inf,
        "is_rational_inf",
        rn_array,
        expected_result,
        n
    );
    return result;
}

/* helper function for keeping things more unified.
Input rational is splitted into two integers
which are passed then to the new_rational_number function.
Beware: malloc inside */
static rational_number new_rational_number_ufunc(
    rational_number a
)
{
    rational_number *b, c;
    b = new_rational_number(a.nominator, a.denominator);
    c = *b;
    free(b);
    return c;
}
static int test_new_rational_number()
{
    const rational_number rn_array[] = {
        {0, 0}, {8, 0}, {625, 1000}, {8*81, -7*5*9}
    };

    int n, result;

    n = ARRAY_SIZE(rn_array);
    result = test_unary_rn_function(
        new_rational_number_ufunc,
        "new_rational_number",
        rn_array,
        rn_array,
        n
    );
    return result;
}

static rational_number standartize_rational_number_ufunc(
    rational_number a
)
{
    rational_number b;
    b = a;
    standartize_rational_number(&b);
    return b;
}
static int test_standartize_rational_number()
{
    const rational_number rn_array[] = {
        {0, 0}, {8, 0}, {625, 1000}, {8*81, -7*5*9}
    };
    const rational_number expected_result[] = {
        {0, 0}, {1, 0}, {5, 8}, {-8*9, 7*5}
    };

    int n, result;

    n = ARRAY_SIZE(rn_array);
    assert(n == ARRAY_SIZE(expected_result));
    result = test_unary_rn_function(
        standartize_rational_number_ufunc,
        "standartize_rational_number",
        rn_array,
        expected_result,
        n
    );
    return result;
}

static int test_add_rationals()
{
    const rational_number rn_a_array[] = {
        {0, 0}, {1, 0}, {4, 5}, {-9, 7}
    };
    const rational_number rn_b_array[] = {
        {0, 0}, {1, 2}, {4, 5}, {9, 83}
    };
    const rational_number expected_result[] = {
        {0, 0}, {9, 0}, {8, 5}, {-684, 581}
    };

    int n, result;

    n = ARRAY_SIZE(rn_a_array);
    assert(n == ARRAY_SIZE(rn_b_array));
    assert(n == ARRAY_SIZE(expected_result));
    result = test_binary_rn_function(
        add_rationals,
        "add_rationals",
        rn_a_array,
        rn_b_array,
        expected_result,
        n
    );
    return result;
}

static int test_subtract_rationals()
{
    const rational_number rn_a_array[] = {
        {0, 0}, {1, 0}, {4, 5}, {0, 7}
    };
    const rational_number rn_b_array[] = {
        {0, 0}, {1, 0}, {4, -5}, {1, 0}
    };
    const rational_number expected_result[] = {
        {0, 0}, {1, 0}, {8, 5}, {1, 0}
    };

    int n, result;

    n = ARRAY_SIZE(rn_a_array);
    assert(n == ARRAY_SIZE(rn_b_array));
    assert(n == ARRAY_SIZE(expected_result));
    result = test_binary_rn_function(
        subtract_rationals,
        "subtract_rationals",
        rn_a_array,
        rn_b_array,
        expected_result,
        n
    );
    return result;
}

static int test_multiply_rationals()
{
    const rational_number rn_a_array[] = {
        {0, 0}, {1, 0}, {4, 5}, {0, 7}
    };
    const rational_number rn_b_array[] = {
        {0, 0}, {1, 0}, {4, -5}, {1, 0}
    };
    const rational_number expected_result[] = {
        {0, 0}, {1, 0}, {-16, 25}, {1, 0}
    };

    int n, result;

    n = ARRAY_SIZE(rn_a_array);
    assert(n == ARRAY_SIZE(rn_b_array));
    assert(n == ARRAY_SIZE(expected_result));
    result = test_binary_rn_function(
        multiply_rationals,
        "multiply_rationals",
        rn_a_array,
        rn_b_array,
        expected_result,
        n
    );
    return result;
}

static int test_divide_rationals()
{
    const rational_number rn_a_array[] = {
        {0, 0}, {1, 0}, {4, 5}, {0, 7}
    };
    const rational_number rn_b_array[] = {
        {0, 0}, {1, 0}, {4, -5}, {1, 0}
    };
    const rational_number expected_result[] = {
        {0, 0}, {0, 0}, {-1, 1}, {0, 0}
    };

    int n, result;

    n = ARRAY_SIZE(rn_a_array);
    assert(n == ARRAY_SIZE(rn_b_array));
    assert(n == ARRAY_SIZE(expected_result));
    result = test_binary_rn_function(
        divide_rationals,
        "divide_rationals",
        rn_a_array,
        rn_b_array,
        expected_result,
        n
    );
    return result;
}


static void inform_about_result(int result)
{
    if (result == 0)
        printf("Success.\n");
    else
        printf("Failed.\n");
}

int main(void) {
    int result;

    printf("Testing: is_rational_equal... ");
    result = test_is_rational_equal();
    inform_about_result(result);

    printf("Testing: is_rational_nan... ");
    result = test_is_rational_nan();
    inform_about_result(result);

    printf("Testing: is_rational_inf... ");
    result = test_is_rational_inf();
    inform_about_result(result);

    printf("Testing: test_new_rational_number... ");
    result = test_new_rational_number();
    inform_about_result(result);

    printf("Testing: test_standartize_rational_number... ");
    result = test_standartize_rational_number();
    inform_about_result(result);

    printf("Testing: test_add_rationals... ");
    result = test_add_rationals();
    inform_about_result(result);

    printf("Testing: test_subtract_rationals... ");
    result = test_subtract_rationals();
    inform_about_result(result);

    printf("Testing: test_multiply_rationals... ");
    result = test_multiply_rationals();
    inform_about_result(result);

    printf("Testing: test_divide_rationals... ");
    result = test_divide_rationals();
    inform_about_result(result);

    return 0;
}
/* Author: Pavel Kharyuk, 2024 */