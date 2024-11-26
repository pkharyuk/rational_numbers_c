#include "handle_input.h"

/* Helper structure for filling the array */
struct rn_array_wrap_st {
    rational_number *rn_array;
    int pos;
    int len;
    int data_pos;
    int sign;
};
typedef struct rn_array_wrap_st rn_array_wrap;

/* Helper structure to work with char buffer */
struct char_buffer_st { 
    char buf[buffer_len];
    int pos;
    int len;
    int seen_data; /* whether a new number is started or not */
};
typedef struct char_buffer_st char_buffer;

/* Initializes the rn_array wrapper */
static rn_array_wrap *create_rn_array_wrapper(int n)
{
    rn_array_wrap *rnw;
    rnw = (rn_array_wrap *) malloc(sizeof(*rnw));
    rnw->rn_array = (rational_number *) malloc(sizeof(rational_number)*n);
    rnw->pos = 0;
    rnw->len = n;
    rnw->data_pos = 0; /* number of a data chunk */
    rnw->sign = 1;
    return rnw;
}
/* Rational number = a / b; a and b are two data chunks here */

/* Initializes the char buffer wrapper */
static void init_char_buffer(char_buffer *cbuf, int buf_len)
{
    if (buf_len > buffer_len) {
        fprintf(
            stderr,
            "Away from the buffer size (%d)\n",
            buffer_len
        );
        exit(EXIT_FAILURE);
    }
    cbuf->pos = 0;
    cbuf->len = buf_len;
    cbuf->seen_data = 0;
}

/* Retrieves the next character in the buffer, increments the position */
static void get_next_char(char *c, char_buffer *cbuf)
{
    *c = cbuf->buf[cbuf->pos];
    cbuf->pos += 1;
}


/* Flush tab/spaces starting from the current buffer position */
static void flush_tabs_and_spaces(char_buffer *cbuf)
{
    while (
        (cbuf->pos < cbuf->len)
        && is_tab_or_space(cbuf->buf[cbuf->pos])
    ) {
        cbuf->pos += 1;
    }
}

/* Read the number of values in the text file */
static void process_first_line(
    char_buffer *cbuf, rn_array_wrap **rnw, int *n
)
{
    char c;
    flush_tabs_and_spaces(cbuf);
    while (cbuf->pos < cbuf->len) {
        get_next_char(&c, cbuf);
        /* the end of the first line: finalize */
        if (c == '\n') {
            if (*n == 0) {
                fprintf(
                    stderr,
                    "No number of rationals specified (1st line)\n"
                );
                exit(EXIT_FAILURE);
            }
            *rnw = create_rn_array_wrapper(*n);
            return;
        }
        /* no other symbols then digits are expected before '\n' */
        if (!is_digit(c)) {
            fprintf(
                stderr,
                "Invalid file contents (non-digit symbol in the 1st line)\n"
            );
            exit(EXIT_FAILURE);
        }
        add_right_digit(n, c);
    }
}

/* Process the current character, updates the rn_array wrapper */
static void update_rn_array_wrapper(
    rn_array_wrap *rnw, char c
)
{
    if (rnw->pos >= rnw->len) {
        fprintf(
            stderr,
            "update_rn_array_wrapper: Invalid rn array position.\n"
        );
        exit(EXIT_FAILURE);
    }
    if (is_sign(c)) {
        rnw->sign *= get_sign(c);
        return;
    }
    if (rnw->data_pos == 0) {
        add_right_digit((int *) &(rnw->rn_array[rnw->pos].nominator), c);
    }
    else if (rnw->data_pos == 1) {
        add_right_digit((int *) &(rnw->rn_array[rnw->pos].denominator), c);
    }
    else {
        fprintf(
            stderr,
            "update_rn_array_wrapper: Invalid data position.\n"
        );
        exit(EXIT_FAILURE);
    }
}

/* Move to processing mext data chunk */
static void start_new_data_chunk(
    char_buffer *cbuf, rn_array_wrap *rnw
)
{
    if (cbuf->seen_data) {
        rnw->data_pos += 1;
        cbuf->seen_data = 0;
    }
}

/* Move to extracting next rational number */
static void to_next_rnw_value(
    rn_array_wrap *rnw
)
{
    rnw->data_pos = 0;
    rnw->rn_array[rnw->pos].nominator *= rnw->sign;
    rnw->sign = 1;
    rnw->pos += 1;
}

/* Character-wise processing of the current char buffer */
static void process_buffer(
    char_buffer *cbuf, rn_array_wrap *rnw
)
{
    char c;
    while (cbuf->pos < cbuf->len) {
        get_next_char(&c, cbuf);
        if (is_tab_or_space(c)) {
            start_new_data_chunk(cbuf, rnw);
            flush_tabs_and_spaces(cbuf);
            continue;
        }
        if (c == '\n') {
            if (cbuf->seen_data) {
                start_new_data_chunk(cbuf, rnw);
            }
            if (rnw->data_pos != expected_data_len) {
                fprintf(
                    stderr,
                    "Invalid input (check the input file, line %d)\n",
                    rnw->pos+2
                );
                exit(EXIT_FAILURE);
            }
            to_next_rnw_value(rnw);
        } else if (is_digit(c)) {
            cbuf->seen_data = 1;
            update_rn_array_wrapper(rnw, c);
        } else if (is_sign(c)) {
            if (cbuf->seen_data) {
                fprintf(
                    stderr,
                    "Invalid input (sign; check the input file, line %d)\n",
                    rnw->pos + 2);
                exit(EXIT_FAILURE);
            }
            cbuf->seen_data = 1;
            update_rn_array_wrapper(rnw, c);
        }
    }
}

/* Reads file and extracts the rational number array from it */
static void read_file(const char *fpath, rational_number **rn_array, int *n)
{
    int fd, n_read, close_status;
    char_buffer cbuf;
    rn_array_wrap *rnw;
    
    fd = open(fpath, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Could not open a file.\n");
        exit(EXIT_FAILURE);
    }

    init_char_buffer(&cbuf, buffer_len);
    rnw = NULL;
    while ((n_read = read(fd, cbuf.buf, buffer_len)) > 0) {
        cbuf.pos = 0;
        cbuf.len = n_read;
        if (rnw == NULL) {
            process_first_line(&cbuf, &rnw, n);
            if (rnw == NULL)
                continue;
        }
        process_buffer(&cbuf, rnw);
    }
    to_next_rnw_value(rnw);
    if (*n != rnw->pos) {
        fprintf(
            stderr,
            "Input file: actual number of lines is smaller than specified\n"
        );
        exit(EXIT_FAILURE);
    }
    *rn_array = rnw->rn_array;
    free(rnw);

    close_status = close(fd);
    if (close_status != 0) {
        fprintf(stderr, "Could not close a file.\n");
        exit(EXIT_FAILURE);
    }
}

/* Abstract view / interface */
void read_rn_array_from_file(
    rational_number **rn_array, int *n, const char *fpath
)
{
    *rn_array = NULL;
    *n = 0;
    read_file(fpath, rn_array, n);
}
/* Author: Pavel Kharyuk, 2024 */