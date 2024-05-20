//
// OS/Assignment/sudoku_setup.c
// Memory Management for Sudoku Solution Validator
// Created by RyanM on 10/05/2024.
//

#include <stdlib.h>
#include "sudoku_setup.h"

/*
Function: initialise_shared_memory
Import: SudokuData* board_info (pointer to the Sudoku board information)
Export: SolutionStruct* (pointer to the initialised solution structure)
Purpose: To initialise the shared memory structures for storing the Sudoku solution and validation results.
*/
// Source: https://github.com/tcbrindle/libsudoku, https://github.com/lordpty/sudoku-solver
SolutionStruct* initialise_shared_memory(SudokuData* board_info) {
    // Allocate memory for the solution structure
    SolutionStruct* solution_info = malloc(sizeof(SolutionStruct));

    // Allocate memory for the 2D array Sol (to store Sudoku solution)
    solution_info->Sol = malloc(board_info->size * sizeof(int*));
    // Allocate memory for Row, Col, and Sub arrays (to store validation results for rows, columns, and sub-grids)
    solution_info->Row = malloc(board_info->size * sizeof(int));
    solution_info->Col = malloc(board_info->size * sizeof(int));
    solution_info->Sub = malloc(board_info->size * sizeof(int));

    solution_info->Counter = 0;

    // Allocate memory for each row in the Sol array
    for (int i = 0; i < board_info->size; i++) {
        solution_info->Sol[i] = malloc(board_info->size * sizeof(int));
    }

    for (int i = 0; i < board_info->size; i++) {
        solution_info->Row[i] = 0;
        solution_info->Col[i] = 0;
        solution_info->Sub[i] = 0;
    }

    return solution_info;
}

/*
Function: free_memory
Import: SolutionStruct* solution_info (pointer to the solution structure),
        SudokuData* board_info (pointer to the Sudoku board information)
Export: void
Purpose: To free the allocated memory for the Sudoku solution and validation results.
*/
// Source: https://github.com/tcbrindle/libsudoku, https://github.com/lordpty/sudoku-solver
void free_memory(SolutionStruct* solution_info, SudokuData* board_info) {
    // Free the memory allocated for each row in the Sol array
    for (int i = 0; i < board_info->size; i++) {
        free(solution_info->Sol[i]);
    }

    // Frees the allocated memory (opposite to function above)
    free(solution_info->Sol);
    free(solution_info->Row);
    free(solution_info->Col);
    free(solution_info->Sub);
    free(board_info);
    free(solution_info); // yay :)
}
