// verbose.c

#include "verbose.h"

bool VERBOSE = true;

void setVerbose(bool verb) { VERBOSE = verb; }

int verbose(const char *restrict format, ...) {
    if (!VERBOSE) {
        return 0;
    }

    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    printf("\n");
    va_end(args);

    return ret;
}

int err_verbose(const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "ERROR: ");
    int ret = vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);

    return ret;
}

int err_verbose_exit(const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "ERROR: ");
    int ret = vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);

    exit(EXIT_FAILURE);
    return ret;
}

// maybe force the flushing of the stdout stream ?
int verbose_no_endline(const char *restrict format, ...) {
    if (!VERBOSE) {
        return 0;
    }

    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);

    return ret;
}

void verbose_endline(void) { printf("\n"); }
