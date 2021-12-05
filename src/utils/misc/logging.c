// logging.c

#include "logging.h"

bool LOGGING_ENABLED = true;

Logger createLogger(const char *file_path) {
    if (file_path == NULL)
        goto noLogFile;
    FILE *fptr = fopen(file_path, "w");
    if (fptr == NULL) {
        err_verbose(
            "Logging file could not be opened. This logger is disabled");
        goto noLogFile;
    }
    return (Logger){
        .file_path = file_path,
        .fptr = fptr,
        .num_logs = 0,
    };

noLogFile:
    return (Logger){
        .file_path = NULL,
        .fptr = NULL,
        .num_logs = 0,
    };
}

void updateLogger(Logger *logger, double value) {
    if (!LOGGING_ENABLED || logger->fptr == NULL)
        return;
    fprintf(logger->fptr, "%lf\n", value);
    logger->num_logs++;
}

void endLogger(Logger *logger) {
    if (!LOGGING_ENABLED || logger->fptr == NULL)
        return;
    fclose(logger->fptr);
}
