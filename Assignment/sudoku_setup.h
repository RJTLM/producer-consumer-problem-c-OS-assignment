//
// Created by RyanM on 20/05/2024.
//

#ifndef SUDOKU_SETUP_H
#define SUDOKU_SETUP_H

// Structure to hold Sudoku board information
typedef struct {
    int size;         // Size of the Sudoku board
    char* sourceFile; // Name of the file containing the Sudoku solution
} SudokuData;

// Structure to hold the Sudoku solution and validation results
typedef struct {
    int** Sol;        // 2D array to store the Sudoku solution
    int* Row;         // Array to store the validation results of each row
    int* Col;         // Array to store the validation results of each column
    int* Sub;         // Array to store the validation results of each sub-grid
    int Counter;      // Counter to keep track of the total number of valid rows, columns, and sub-grids
} SolutionStruct;

// Function prototypes for initialising and freeing shared memory
SolutionStruct* initialise_shared_memory(SudokuData* board_info);
void free_memory(SolutionStruct* solution_info, SudokuData* board_info);

#endif
