/* This module is designed for extracting a
rational number array from the input file by
a character-wise processing. */

#ifndef __PROCESS_STRING_H__
#define __PROCESS_STRING_H__

#include <stdio.h>
#include <stdlib.h>

enum { ten = 10 };

#include "process_string.h"

/* Converts digit as character to the corresponding integer value */
int digit_char_to_int(char c);

/* Step up the number filling procedure
 a = 10*a + c */
void add_right_digit(int *a, char c);

/* Check whether a char is a digit */
int is_digit(char c);

/* Check whether a char is a plus or minus */
int is_sign(char c);

/* Convert sign character to a +/- 1 */
int get_sign(char c);

/* Check whether a character is a tab or space */
int is_tab_or_space(char c);

/* Safe implementaion of atoi function */
int safe_atoi(const char *buf);

#endif
/* Author: Pavel Kharyuk, 2024 */