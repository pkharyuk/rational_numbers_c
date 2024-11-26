/* This module is designed for extracting a
rational number array from the input file by
a character-wise processing. */

#ifndef __HANDLE_CLARGS_H__
#define __HANDLE_CLARGS_H__

#include <stdio.h>
#include <stdlib.h>

enum input_mode {
    load_from_file_mode,
    generate_random_mode,
    n_input_modes
};

enum { expected_argc = 3 };

enum input_mode check_cl_args(int argc, const char *argv[]);

#endif
/* Author: Pavel Kharyuk, 2024 */