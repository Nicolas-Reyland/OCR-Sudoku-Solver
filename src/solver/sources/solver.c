/* Source solver.c
 *
 * Source code that defines the solver functionnality.
 * Function is given a integer prefilled matrix.
 *
 * by Lilian Schall 2021 Sudoku Solver Epita
 */

#include "utils/verbosity/verbose.h"
#include <stdlib.h>

#include "./../headers/const.h"
#include "./../headers/tester.h"

// prototypes of functions used to check wheter a number is already used
// in the column, square and line selected
int already_in_column(int x, int val, int matrix[SIZE][SIZE]);
int already_in_line(int y, int val, int matrix[SIZE][SIZE]);
int already_in_square(int x, int y, int val, int matrix[SIZE][SIZE]);

// public function used to solve a sudoku matrix
int solver(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == 0) {
                for (int n = 1; n < 10; n++) {
                    // sudoku rule, a number can only be one time
                    // in a predefined square, line and column
                    // at the same time
                    if (!already_in_line(i, n, matrix) &&
                        !already_in_column(j, n, matrix) &&
                        !already_in_square(j, i, n, matrix)) {
                        matrix[i][j] = n;
                        if (solver(matrix))
                            return 1;
                        // backtracking solution, if every combination of
                        // numbers didn't filled our matrix well, we have to go
                        // back and try another number
                        matrix[i][j] = 0;
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}

// private function used in solver to detect if there is already
// a number in the selected column
// returns bool
int already_in_column(int x, int val, int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        if (matrix[i][x] == val)
            return 1;
    }
    return 0;
}

// private function used in solver to detect if there is already
// a number in the selected line
// returns bool
int already_in_line(int y, int val, int matrix[SIZE][SIZE]) {
    for (int j = 0; j < SIZE; j++) {
        if (matrix[y][j] == val)
            return 1;
    }
    return 0;
}

// private function used in solver to detect if there is already
// a number in the selected square
// returns bool
int already_in_square(int x, int y, int val, int matrix[SIZE][SIZE]) {
    y = y / 3 * 3;
    x = x / 3 * 3;
    for (int i = y; i < y + 3; i++) {
        for (int j = x; j < x + 3; j++) {
            if (matrix[i][j] == val)
                return 1;
        }
    }
    return 0;
}
