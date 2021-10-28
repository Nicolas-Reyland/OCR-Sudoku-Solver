// verbose.h

#ifndef UTILS_VERBOSITY_VERBOSE_H
#define UTILS_VERBOSITY_VERBOSE_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdarg.h>

// bool VERBOSE;

int verbose(const char * restrict, ...);
int verbose_no_endline(const char * restrict, ...);
void setVerbose(bool);

#endif
