// verbose.c

#include "verbose.h"

void setVerbose(bool verb)
{
    VERBOSE = verb;
}

bool VERBOSE = true;

int verbose(const char * restrict format, ...) {
    if ( !VERBOSE ) {
        return 0;
    }

    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);

    return ret;
}
