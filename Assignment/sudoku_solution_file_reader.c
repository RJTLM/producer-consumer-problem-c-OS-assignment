//
// Created by RyanM on 20/05/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solution_file_reader.h"
#include "sudoku_setup.h"

/*
Function: load_sudoku_from_file
Import: SudokuData* board_data (pointer to the Sudoku board information),
        SolutionStruct* solution_data (pointer to the solution structure)
Export: int (0 for success, 1 for error)
Purpose: To read a Sudoku solution from a file and store it in the provided solution structure.
*/
// Source: https://github.com/tcbrindle/libsudoku, https://github.com/lordpty/sudoku-solver
int load_sudoku_from_file(SudokuData* board_data, SolutionStruct* solution_data) {
    // Open the file containing the Sudoku solution
    FILE* file_pointer = fopen(board_data->sourceFile, "r");
    if (file_pointer == NULL) {
        // Print an error message if the file cannot be opened
        fprintf(stderr, "\033[31mError\033[0m\t Unable to open the specified file\n");
        return 1;
    }

    // Iterate through the Sudoku board rows
    for (int row = 0; row < board_data->size; row++) {
        // Iterate through the Sudoku board columns
        for (int col = 0; col < board_data->size; col++) {
            int num;
            // Read each number from the file
            if ((fscanf(file_pointer, "%d", &num) != 1) || (num < 1 || num > 9)) {
                // Print an error message if the number is invalid
                fprintf(stderr, "\033[31mError\033[0m\t Invalid data in the file\n");
                fclose(file_pointer); // Close the file
                return 1;
            }
            // Store the read number in the solution structure
            solution_data->Sol[row][col] = num;
        }
    }

    // Close the file after reading all data
    fclose(file_pointer);
    return 0; // Return 0 to indicate success
}
