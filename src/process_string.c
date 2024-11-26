#include "process_string.h"

/* Converts digit as character to the corresponding integer value */
int digit_char_to_int(char c)
{
    return (int)(c) - (int)('0');
}

/* Step up the number filling procedure
 a = 10*a + c */
void add_right_digit(int *a, char c)
{
    *a *= ten;
    *a += digit_char_to_int(c);
}

/* Check whether a char is a digit */
int is_digit(char c) 
{
    return (c >= '0' && c <= '9');
}

/* Check whether a char is a plus or minus */ 
int is_sign(char c)
{
    return (c == '-' || c == '+');
}

/* Convert sign character to a +/- 1 */
int get_sign(char c)
{
    if (c == '-')
        return -1;
    if (c == '+')
        return 1;
    fprintf(
        stderr,
        "get_sign: not a sign character passed"
    );
    exit(EXIT_FAILURE);
}

/* Check whether a character is a tab or space */
int is_tab_or_space(char c)
{
    return (c == ' ' || c == '\t');
}

/* Safe implementaion of atoi function */
int safe_atoi(const char *buf)
{
    int ans;
    char *c;

    ans = 0;
    for (c = (char *) buf; *c != '\0'; c++) {
        if (!is_digit(*c)) {
            fprintf(
                stderr,
                "safe_atoi: non-digit character passed.\n"
            );
            exit(EXIT_FAILURE);
        }
        add_right_digit(&ans, *c);
    }
    return ans;
}
/* Author: Pavel Kharyuk, 2024 */