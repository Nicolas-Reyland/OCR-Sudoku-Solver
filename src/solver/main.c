/* Source main.c
 *
 * Enterpoint of program, call reader solver and writer functionnality
 * After compilation, program can be call as CLI
 *
 * Command format is 
 * ./[name_of_executable] [name_of_sudoku_grid]
 * and will output in the current folder:
 * [name_of_sudoku_grid].output
 *
 * By Lilian Schall 2021 Sudoku Solver Epita
*/


#include "utils/verbosity/nn_verbose.h"
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "./headers/reader.h"
#include "./headers/solver.h"
#include "./headers/const.h"
#include "./headers/writer.h"

int main(int argc, char * argv[])
{
    if(argc != 2)
        errx(1,"Error: one argument is required.");
    
    char* arg = argv[1];

    int matrix[SIZE][SIZE];
    
    reader(arg,matrix);
    solver(matrix);
    strcat(arg,".output");
    writer(arg,matrix);
    
    return 0;
}
