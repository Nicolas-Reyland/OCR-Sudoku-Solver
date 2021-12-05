// progressbar.h

#ifndef UTILS_VERBOSITY_PROGRESSBAR_H
#define UTILS_VERBOSITY_PROGRESSBAR_H

#include "verbose.h"

#define PROGRESSBAR_PASSED '='
#define PROGRESSBAR_REMAIN '-'
#define DEFAULT_PROGRESSBAR_WIDTH 100

extern bool PROGRESS_BAR_STATUS;

typedef struct ProgressBar ProgressBar;
struct ProgressBar {
    const char *name;
    size_t start;
    size_t end;
    size_t range;
    size_t state;
    size_t width;
    size_t drawn;
    float percentage;
};

ProgressBar createProgressBar(const char *name, size_t start, size_t end,
                              size_t width);
void updateProgressBar(ProgressBar *bar, size_t state);
void endProgressBar(ProgressBar *bar);

#endif
