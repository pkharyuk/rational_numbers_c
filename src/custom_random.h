/*
    Functions to perform custom random draws.
    If seed is a NULL pointer, functions reuse the current random state.
*/
#ifndef __CUSTOM_RANDOM_H__
#define __CUSTOM_RANDOM_H__

#include <math.h>
#include <stdlib.h>
#include <assert.h>

/* custom type definitions */
typedef unsigned int seed_t;

/* Random integer draw from Cat{a, a+1, ..., b-1, b}, a < b distribution.
p(a) = p(a+1) = ... = p(b-1) = p(b) = 1/(b-a+1) */
int randint_random_draw(seed_t *seed, int a, int b);

/* Random draw from U[0, 1] */
double uniform_random_draw(seed_t *seed);

/* Ratio-of-Uniforms method to draw a sample from N(0, 1)
see, e.g.,
https://www2.econ.osaka-u.ac.jp/~tanizaki/class/2013/econome3/13.pdf */
double standard_normal_random_draw(seed_t *seed);

/* N(\mu, \sigma^2) */
double normal_random_draw(
    seed_t *seed, double mu, double sigma
);

/* Log-Normal(\mu, \sigma^2) */
double lognormal_random_draw(
    seed_t *seed, double mu, double sigma
);

#endif
/* Author: Pavel Kharyuk, 2024 */