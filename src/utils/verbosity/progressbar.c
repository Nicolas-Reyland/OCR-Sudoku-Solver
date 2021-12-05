// progressbar.c

#include "progressbar.h"

/*
printf("\033[XA"); // Move up X lines;
printf("\033[XB"); // Move down X lines;
printf("\033[XC"); // Move right X column;
printf("\033[XD"); // Move left X column;
printf("\033[2J"); // Clear screen
*/

bool PROGRESS_BAR_STATUS = true;

ProgressBar createProgressBar(const char *name, size_t start, size_t end,
                              size_t width) {
    ProgressBar bar = {
        .name = name,
        .start = start,
        .end = end,
        .range = end - start,
        .state = start,
        .width = width,
        .drawn = 0,
        .percentage = 0.0,
    };
    if (name == NULL || !PROGRESS_BAR_STATUS)
        return bar;

    printf("%s : [", name);
    for (size_t i = 0; i < width; i++) {
        putchar(PROGRESSBAR_REMAIN);
    }
    printf("] %.2f%c", /*100.0 * (is zero anyway) */ bar.percentage, '%');
    fflush(stdout);

    return bar;
}

void updateProgressBar(ProgressBar *bar, size_t state) {
    if (!PROGRESS_BAR_STATUS)
        return;

    float new_percentage = (float)state / (float)bar->range;
    size_t should_drawn = new_percentage * (float)bar->width;
    if (should_drawn != bar->drawn) {
        size_t num_chars = should_drawn - bar->drawn;
        // cursor to end of progress bar
        printf("\033[%luD", bar->width + 7 - bar->drawn);
        // fill the progress bar
        for (size_t i = 0; i < num_chars; i++)
            putchar(PROGRESSBAR_PASSED);
        // cursor to percentage
        printf("\033[%luC", bar->width - should_drawn + 2);
        // update the percentage
        printf("%.2f%c", 100.0 * new_percentage, '%');
        if (new_percentage >= 0.1) {
            printf("\033[1D");
        }

        fflush(stdout);
        bar->drawn = should_drawn;
    }
    bar->state = state;
    bar->percentage = new_percentage;
}

void endProgressBar(ProgressBar *bar) {
    if (bar->name == NULL || !PROGRESS_BAR_STATUS)
        return;
    // go to beginning of the line
    printf("\r%s : [", bar->name);
    for (size_t i = 0; i < bar->width; i++) {
        putchar(PROGRESSBAR_PASSED);
    }
    printf("] 100%c", '%');
    printf(" (done)\n");
}
