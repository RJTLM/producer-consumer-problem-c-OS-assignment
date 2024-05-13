//
// OS/Assignment/thread_functions.c
// Utility functions for the Sudoku Solution Validator
// Created by RyanM on 11/05/2024.
//

/*
 * This module is where the core logic of the threading application resides.
 * It contains the functions that each thread will execute.
 * These functions are responsible for performing the actual validation work on the Sudoku grid,
 * whether it's checking rows, columns, or sub-grids.
 * The functions implemented (validateRowsAndColumns and validateSubGrids) are structured
 * to perform their tasks based on the parameters passed through ThreadArgs.
 */

#include <stdio.h>
#include "thread_functions.h"
#include "utility.h"

// Helper function to check if all elements are present in a section
int validateSection(int section[9]) {
    int seen[10] = {0};  // Using an array to track numbers from 1 to 9
    for (int i = 0; i < 9; i++) {
        if (section[i] < 1 || section[i] > 9 || seen[section[i]]) {
            return 0;  // Return false if an invalid number or a repeat is found
        }
        seen[section[i]] = 1;
    }
    return 1;  // Return true if the section is valid
}

// In thread_functions.c
void *validateRowsAndColumns(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;
    int valid = 1;
    char buffer[256];

    pthread_mutex_lock(thread_args->mutex); // Ensure mutex lock is properly called

    if (thread_args->validate_rows) {
        for (int i = thread_args->start_row; i <= thread_args->end_row; i++) {
            if (!validateSection(thread_args->sudoku[i])) {
                valid = 0;
                sprintf(buffer, "Thread ID-%d: Row %d validation failed.", thread_args->thread_id, i + 1);
                printFailure(buffer);
                break;
            }
        }
    } else {
        for (int col = 0; col < 9; col++) {
            int column[9];
            for (int row = 0; row < 9; row++) {
                column[row] = thread_args->sudoku[row][col];
            }
            if (!validateSection(column)) {
                valid = 0;
                sprintf(buffer, "Thread ID-%d: Column %d validation failed.", thread_args->thread_id, col + 1);
                printFailure(buffer);
                break;
            }
        }
    }

    sprintf(buffer, "Thread ID-%d: %s validation completed with %s.", thread_args->thread_id, thread_args->validate_rows ? "Row" : "Column", valid ? "success" : "failure");
    if (valid) {
        printSuccess(buffer);
    } else {
        printFailure(buffer);
    }

    printf("Thread ID-%d: %s validation completed with %s.\n", thread_args->thread_id, thread_args->validate_rows ? "Row" : "Column", valid ? "success" : "failure");
    pthread_mutex_unlock(thread_args->mutex); // Ensure mutex unlock is properly called
    pthread_exit(NULL); // Correct exit from thread
}

void *validateSubGrids(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;
    int valid = 1;

    // Define sub-grid boundaries and validate
    int startRow = (thread_args->thread_id % 3) * 3;
    int startCol = (thread_args->thread_id / 3) * 3;
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            int grid[9];
            int k = 0;
            for (int row = i; row < i + 3; row++) {
                for (int col = j; col < j + 3; col++) {
                    grid[k++] = thread_args->sudoku[row][col];
                }
            }
            if (!validateSection(grid)) {
                valid = 0;
                break;
            }
        }
        if (!valid) break;
    }

    pthread_mutex_lock(thread_args->mutex);
    printf("Thread ID-%d: Sub-grid validation completed with %s.\n", thread_args->thread_id, valid ? "success" : "failure");
    pthread_mutex_unlock(thread_args->mutex);

    pthread_exit(NULL);
}
