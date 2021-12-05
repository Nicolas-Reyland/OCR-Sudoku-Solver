/* Source writer.c
 *
 * Source code that write a sudoky grid into a formatted textfile
 * Textfile is formatted as follows:
 *
 * ... ... ...
 * ... ... ...
 * ... ... ...
 *
 * ... ... ...
 * etc..
 *
 * When textpath is incorrect, program is exited with exit status 1
 *
 * by Lilian Schall Sudoku Solver 2021 Epita
 */

#include "./../headers/const.h"
#include "utils/verbosity/verbose.h"
#include <err.h>
#include <stdlib.h>

void writer(char *filename, int matrix[SIZE][SIZE]) {
    FILE *fichier = fopen(filename, "w+");

    if (fichier != NULL) {
        // boolean used to define when we have to put
        // a formatting character like '\n' or ' '
        int notJumped = 0;

        for (int i = 0; i < SIZE; i++) {
            if (i % 3 == 0 && notJumped) {
                // when we have written 3 lines, we should
                // let one line clear by putting '\n'
                // character
                notJumped = 0;
                i--;
                fputc('\n', fichier);
                continue;
            }
            for (int j = 0; j < SIZE; j++) {
                if (j % 3 == 0 && j != 0 && notJumped) {
                    // when we have written 3 numbers, we should
                    // let one space clear by putting ' '
                    // character
                    notJumped = 0;
                    j--;
                    fputc(' ', fichier);
                    continue;
                }
                // adding ascii offset to integer
                // so it represents the character and not the number
                fputc(((char)(matrix[i][j] + '0')), fichier);
                notJumped = 1;
            }
            notJumped = 1;
            fputc('\n', fichier);
        }
        fclose(fichier);
    } else
        errx(1, "Error, Output File is missing");
}
