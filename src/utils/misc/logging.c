// logging.c

#include "logging.h"

bool LOGGING_ENABLED = true;

Logger createLogger(const char* file_path)
{
    FILE* fptr = fopen(file_path, "w");
    if (fptr == NULL) {
        err_verbose("Logging file could not be opened. Disabling this logger");
        return (Logger) {
            .file_path = NULL,
            .fptr = NULL,
            .num_logs = 0,
        };
    }
    return (Logger) {
        .file_path = file_path,
        .fptr = fptr,
        .num_logs = 0,
    };
}

void updateLogger(Logger* logger, double value)
{
    if (!LOGGING_ENABLED || logger->fptr == NULL)
        return;
    fprintf(logger->fptr, "%lf\n", value);
    logger->num_logs++;
}

void endLogger(Logger* logger)
{
    if (!LOGGING_ENABLED || logger->fptr == NULL)
        return;
    fclose(logger->fptr);
}

