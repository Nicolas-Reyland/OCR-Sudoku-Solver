// logging.h

#ifndef UTILS_MISC_LOGGING_H
#define UTILS_MISC_LOGGING_H

#include <stdlib.h>
#include <stdbool.h>
#include "utils/verbosity/verbose.h"

extern bool LOGGING_ENABLED;

typedef struct Logger Logger;
struct Logger
{
    const char* file_path;
    FILE *fptr;
    size_t num_logs;
};

Logger createLogger(const char* file_path);
void updateLogger(Logger* logger, double value);
void endLogger(Logger* logger);

#endif
