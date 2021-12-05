/* Source reader.c
 *
 * Source code that read a formatted textfile representing a sudoku grid
 * and transform it into a SIZE*SIZE matrix.
 *
 * Textfile is formatted as follows:
 *
 * ... ... ...
 * ... ... ...
 * ... ... ...
 *
 * ... ... ...
 * etc..
 *
 * When textfile is poorly formatted, program is exited with exit status 1
 * and reason is given.
 *
 * by Lilian Schall Sudoku Solver 2021 Epita
 */

#include "../headers/const.h"
#include "utils/verbosity/verbose.h"
#include <err.h>
#include <stdlib.h>

void reader(char *filepath, int matrix[SIZE][SIZE]) {
    // get pointer to file to process (read-only)
    FILE *file = fopen(filepath, "r+");

    if (file != NULL) {
        char car = 0;
        int buff = 0;

        // bool that is used to escape the char that is used to format
        // textfile (generally ' ' or '\n')
        int hasJumped = 1;

        for (int i = 0; i < SIZE; i++) {
            if (i % 3 == 0 && !hasJumped) {
                // Escape the '\n' after each three lines
                car = fgetc(file);
                hasJumped = 1;
                i--;
                continue;
            }
            for (int j = 0; j < SIZE; j++) {
                car = fgetc(file);
                if (j % 3 == 0 && car == ' ') {
                    // escape the ' ' every three character read
                    j--;
                    continue;
                }
                if (car >= '1' && car <= '9') {
                    // get the rid of the ascii character offset
                    buff = ((int)(car - '0'));
                } else if (car == '.') {
                    buff = 0;
                } else {
                    // char is not recognized, which means
                    //  bad formatting
                    errx(1, "error: file is poorly formatted");
                }
                // put the readen character into the matrix
                matrix[i][j] = buff;
            }
            car = fgetc(file); // escape the '\n' at end of line
            hasJumped = 0;
        }
    } else
        errx(1, "error: filepath is wrong");
}
