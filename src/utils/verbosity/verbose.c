#include "verbose.h"
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#define VERBOSE false

int verbose(const char * restrict format, ...) {
    if( !VERBOSE )
        return 0;

    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);

    return ret;
}
