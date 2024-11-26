/*
Rational numbers demo.

The following demo code either generates n random samples
or reads rational numbers from the text file (see sample_input.txt)
and computes their mean.

Usage: ./<program_name> -i <path_to_file>
OR ./<program_name> -n <n_numbers>

Known issues:
- no handling of overful cases ;

Random sampling procedure is dependent on sampling hyperparameters,
which should be selected properly.
*/

#include <stdio.h>
#include "custom_random.h"
#include "handle_input.h"
#include "handle_clargs.h"
#include "process_string.h"
#include "rational_number.h"

enum { numbers_per_line = 5 };
enum { demo_n = 10 };
enum { random_seed = 775 };

static const double norm_mu = 0;
static const double norm_sigma = 19.7;
static const double lognorm_mu = 2.;
static const double lognorm_sigma = 0.5;

static int custom_round(double a)
{
    return (int) (a + 0.5);
}

static void draw_random_rationals(
    seed_t *seed, int n, rational_number **rn_array
)
{
    int i;
    double sample;

    *rn_array = (rational_number *) malloc(sizeof(rational_number) * n);
    for (i = 0; i < n; i++) {
        sample = normal_random_draw(seed, norm_mu, norm_sigma);
        (*rn_array)[i].nominator = custom_round(sample);
        sample = lognormal_random_draw(seed, lognorm_mu, lognorm_sigma);
        (*rn_array)[i].denominator = custom_round(sample);
    }
}

static rational_number compute_mean_rn(rational_number *rn_array, int n)
{
    rational_number result, rn_r;
    result = reduce_rationals(add_rationals, rn_array, n);
    rn_r = int_to_rational_number(n);
    return divide_rationals(result, rn_r);
}

static void print_result(rational_number *rn_array, int n)
{
    int i;
    rational_number result;

    result = compute_mean_rn(rn_array, n);

    printf("1 / %d [\n\t", n);
    for (i = 0; i < n; i++) {
        printf("(");
        print_rational_number(rn_array[i]);
        printf(")");
        if (i < (n-1)) {
            printf(" + ");
            if ((i + 1) % numbers_per_line == 0)
                printf("\n\t");
        }
    }
    printf("\n] = ");
    print_rational_number(result);
    printf("\n");
}

int main(int argc, char *argv[]) 
{
    rational_number *rn_array;
    int n;
    enum input_mode mode;
    seed_t rseed = random_seed;

    mode = check_cl_args(argc, (const char **) argv);
    switch (mode) {
        case load_from_file_mode:
            read_rn_array_from_file(&rn_array, &n, (const char *) argv[2]);
            break;
        case generate_random_mode:
            n = safe_atoi((const char *) argv[2]);
            draw_random_rationals(&rseed, n, &rn_array);
            break;
        default:
            fprintf(stderr, "Invalid input mode\n");
            exit(EXIT_FAILURE);
        }

    print_result(rn_array, n);
    free(rn_array);

    return 0;
}
/* Author: Pavel Kharyuk, 2024 */