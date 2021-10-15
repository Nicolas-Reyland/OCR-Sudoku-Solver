// verbose.h

#ifndef UTILS_VERBOSITY_VERBOSE_H
#define UTILS_VERBOSITY_VERBOSE_H

#include <stdbool.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

int verbose(const char * restrict, ...);
void setVerbose(bool);

#endif
