//
// OS/Assignment/suduko_validator.c
// Implementation of Sudoku validation functions
// Created by RyanM on 30/04/2024.
//

#include "mssv.h"
#include <stdbool.h>
#include <stdio.h>

// Helper function to check if all elements are present exactly once
static bool check_digits(const int section[]) {
    bool seen[10] = {false};  // Track digits 1-9; ignore index 0
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        int val = section[i];
        if (val < 1 || val > 9 || seen[val]) {
            return false;  // Invalid value or duplicate
        }
        seen[val] = true;
    }
    return true;
}

// Validates rows from start to end
void validate_rows(int start, int end) {
    printf("Validating rows from %d to %d\n", start, end);
    for (int i = start; i <= end; i++) {
        pthread_mutex_lock(&mutex);
        rows[i] = check_digits(sudokuGrid[i]) ? 1 : 0;
        pthread_mutex_unlock(&mutex);
    }
}

// Validates columns from start to end
void validate_columns(int start, int end) {
    printf("Validating columns from %d to %d\n", start, end);
    for (int j = start; j <= end; j++) {
        int column[SUDOKU_SIZE];
        for (int i = 0; i < SUDOKU_SIZE; i++) {
            column[i] = sudokuGrid[i][j];
        }
        pthread_mutex_lock(&mutex);
        columns[j] = check_digits(column) ? 1 : 0;
        pthread_mutex_unlock(&mutex);
    }
}

// Validates 3x3 subgrids from start to end
void validate_subgrids(int start, int end) {
    printf("Validating subgrids from %d to %d\n", start, end);
    for (int index = start; index <= end; index++) {
        int rowStart = (index / 3) * 3;
        int colStart = (index % 3) * 3;
        int subgrid[SUDOKU_SIZE];
        int idx = 0;
        for (int i = rowStart; i < rowStart + 3; i++) {
            for (int j = colStart; j < colStart + 3; j++) {
                subgrid[idx++] = sudokuGrid[i][j];
            }
        }
        pthread_mutex_lock(&mutex);
        subgrids[index] = check_digits(subgrid) ? 1 : 0;
        pthread_mutex_unlock(&mutex);
    }
}
