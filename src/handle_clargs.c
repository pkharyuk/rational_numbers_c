#include "handle_clargs.h"

/* Initial checker of CL arguments */
enum input_mode check_cl_args(int argc, const char *argv[])
{
    enum input_mode result;
    int option_ind;

    if (argc != expected_argc) {
        fprintf(
            stderr,
            "Usage: -i <file_path> OR -n <random_array_length>\n"
        );
        exit(EXIT_FAILURE);
    }
    for (option_ind = 1; option_ind < argc; option_ind++) {
        if (argv[option_ind][0] != '-')
            break;
        switch (argv[option_ind][1]) {
            case 'i':
                result = load_from_file_mode;
                break;
            case 'n':
                result = generate_random_mode;
                break;
            default:
                fprintf(
                    stderr,
                    "Invalid option\n"
                );
                exit(EXIT_FAILURE);
        }
        option_ind += 1;
    }
    return result;
}
/* Author: Pavel Kharyuk, 2024 */