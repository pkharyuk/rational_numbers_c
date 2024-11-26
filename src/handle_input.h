/* This module is designed for extracting a
rational number array from the input file by
a character-wise processing. */

#ifndef __HANDLE_INPUT_H__
#define __HANDLE_INPUT_H__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>

#include "process_string.h"
#include "rational_number.h"

enum { expected_data_len = 2 };
enum { buffer_len = 1024 };

void read_rn_array_from_file(
    rational_number **rn_array, int *n, const char *fpath
);

#endif
/* Author: Pavel Kharyuk, 2024 */