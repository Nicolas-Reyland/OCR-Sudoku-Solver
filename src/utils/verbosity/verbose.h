// verbose.h

#ifndef UTILS_VERBOSITY_VERBOSE_H
#define UTILS_VERBOSITY_VERBOSE_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdarg.h>

extern bool VERBOSE;

int verbose(const char * restrict, ...);
int verbose_no_endline(const char * restrict, ...);
int err_verbose(const char * restrict, ...);
int err_verbose_exit(const char * restrict, ...);
void verbose_endline(void);
void setVerbose(bool);

#endif
