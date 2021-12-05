/* Source tester.c
 *
 * Source code used to run tests on our filled matrix.
 * public function is_solved test if the given sudoku matrix
 * is solved based on the rules of the game
 *
 * by Lilian Schall 2021 Sudoku Solver Epita
 */

#include "./../headers/const.h"
#include "utils/verbosity/verbose.h"

// prototypes that define private function used in is_solved
int is_column_solved(int x, int matrix[SIZE][SIZE]);
int is_line_solved(int y, int matrix[SIZE][SIZE]);
int is_square_solved(int x, int y, int matrix[SIZE][SIZE]);

// public function that test wheter a sudoku matrix is solved or not
// receives a filled sudoku matrix in argument
// returns a bool
int is_solved(int matrix[SIZE][SIZE]) {
    int test = 0;

    // linear test scale, we test at first every line and column together
    for (int i = 0; i < SIZE; i++) {
        test = is_column_solved(i, matrix);
        test = test && is_line_solved(i, matrix);

        // we interrupt the testing when we find one column / lines with an
        // error.
        if (!test)
            return 0;
    }

    // linear test scale, we test every square one by one (9 square in total)
    for (int i = 0; i < SIZE; i += 3) {
        for (int j = 0; j < SIZE; j += 3) {
            test = is_square_solved(i, j, matrix);
            if (!test)
                return 0;
        }
    }
    return 1;
}

// private function that test if every number from 1 to 9
// is found in a column only one time
// returns bool
int is_column_solved(int x, int matrix[SIZE][SIZE]) {
    int verification[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int check;
    for (int i = 0; i < SIZE; i++) {
        check = matrix[i][x];
        if (check == 0 || verification[check] == 1)
            return 0;
        verification[check] = 1;
    }
    return 1;
}

// private function that test if every number from 1 to 9
// is found in a line only one time
// returns bool
int is_line_solved(int y, int matrix[SIZE][SIZE]) {
    int verification[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int check;
    for (int j = 0; j < SIZE; j++) {
        check = matrix[y][j];
        if (check == 0 || verification[check] == 1)
            return 0;
        verification[check] = 1;
    }
    return 1;
}

// private function that test if every number from 1 to 9
// is found in a square only one time
// returns bool
int is_square_solved(int x, int y, int matrix[SIZE][SIZE]) {
    int verification[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int check;
    y = y / 3 * 3;
    x = x / 3 * 3;
    for (int i = y; i < y + 3; i++) {
        for (int j = x; j < x + 3; j++) {
            check = matrix[i][j];
            if (check == 0 || verification[check] == 1)
                return 0;
            verification[check] = 1;
        }
    }
    return 1;
}
