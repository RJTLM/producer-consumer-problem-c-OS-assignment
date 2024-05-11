//
// OS/Assignment/utilities.c
// Utility functions for the Sudoku Solution Validatorplease update the header files
// Created by RyanM on 30/04/2024.
//

#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>

// Reads a Sudoku grid from a file
int** read_sudoku_grid(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    } else {
        printf("File opened successfully: %s\n", filename);
    }

    int** grid = malloc(9 * sizeof(int*));
    for (int i = 0; i < 9; i++) {
        grid[i] = malloc(9 * sizeof(int));
        for (int j = 0; j < 9; j++) {
            fscanf(file, "%d", &grid[i][j]);
        }
    }

    fclose(file);
    return grid;
}

// Frees a Sudoku grid
void free_sudoku_grid(int** grid) {
    for (int i = 0; i < 9; i++) {
        free(grid[i]);
    }
    free(grid);
}

// Additional utility functions as needed
