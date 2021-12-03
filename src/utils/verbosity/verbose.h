// nn_verbose.h

#ifndef UTILS_VERBOSITY_VERBOSE_H
#define UTILS_VERBOSITY_VERBOSE_H

#define _GNU_SOURCE
// #define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

extern bool VERBOSE;

int nn_verbose(const char * restrict, ...);
int nn_verbose_no_endline(const char * restrict, ...);
int nn_err_nn_verbose(const char * restrict, ...);
int nn_err_nn_verbose_exit(const char * restrict, ...);
void nn_verbose_endline(void);
void nn_setVerbose(bool);

#endif
