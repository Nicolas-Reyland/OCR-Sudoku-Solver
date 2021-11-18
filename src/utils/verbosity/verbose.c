// verbose.c

#include "verbose.h"

bool VERBOSE = true;

void setVerbose(bool verb)
{
    VERBOSE = verb;
}

int verbose(const char * restrict format, ...) {
    if ( !VERBOSE ) {
        return 0;
    }

    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    printf("\n");
    va_end(args);

    return ret;
}

// maybe force the flushing of the stdout stream ?
int verbose_no_endline(const char * restrict format, ...) {
    if ( !VERBOSE ) {
        return 0;
    }

    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);

    return ret;
}

void verbose_endline(void)
{
    printf("\n");
}
