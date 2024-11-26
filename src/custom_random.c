#include "custom_random.h"

/* U[a, b] */
int randint_random_draw(seed_t *seed, int a, int b)
{
    int rv;
    assert (b > a);
    if (seed != NULL)
        srand(*seed);
    rv = a + rand() / (RAND_MAX / (b - a));
    if (seed != NULL)
        *seed = (seed_t) rand();
    return rv;
}

/* U[0, 1] */
double uniform_random_draw(seed_t *seed)
{
    double rv;
    if (seed != NULL)
        srand(*seed);
    rv = ((double) rand()) / RAND_MAX;
    if (seed != NULL)
        *seed = (seed_t) rand();
    return rv;
}

/* Ratio-of-Uniforms method to draw a sample from N(0, 1)
see, e.g.,
https://www2.econ.osaka-u.ac.jp/~tanizaki/class/2013/econome3/13.pdf */
double standard_normal_random_draw(seed_t *seed)
{
    double u, v;

    u = 1.;
    v = 1.;
    while (-4.*u*u*log(u) < v*v) {
        u = uniform_random_draw(seed);
        v = uniform_random_draw(seed);
        v = (2.*v - 1.)*sqrt(2.*exp(-1.));
    }
    return v / u;
}

/* N(\mu, \sigma^2) */
double normal_random_draw(
    seed_t *seed, double mu, double sigma
)
{
    assert (sigma >= 0.);
    return mu + sigma * standard_normal_random_draw(seed);
}

/* Log-Normal(\mu, \sigma^2) */
double lognormal_random_draw(
    seed_t *seed, double mu, double sigma
)
{
    double s;

    s = normal_random_draw(seed, mu, sigma);
    return exp(s);
}

/* Author: Pavel Kharyuk, 2024 */