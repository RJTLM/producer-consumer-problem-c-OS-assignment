//
// OS/Assignment/utility.c
// Utility functions for the Sudoku Solution Validatorplease update the header files
// Created by RyanM on 30/04/2024.
//

#include "utility.h"

void printErrorMessage(char *message) {
    fprintf(stderr, "Error: %s\n", message);
}

void printMessage(char *message) {
    printf("%s\n", message);
}

void printSudokuSolution(int sudoku[9][9]) {
    printf("Sudoku Solution:\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
}
