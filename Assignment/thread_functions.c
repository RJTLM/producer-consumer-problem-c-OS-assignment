//
// OS/Assignment/thread_functions.c
// Utility functions for the Sudoku Solution Validator
// Created by RyanM on 11/05/2024.
//

#include "thread_functions.h"

void *validateRowsAndColumns(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;
    int (*sudoku)[9] = thread_args->sudoku;
    int start_row = thread_args->start_row;
    int end_row = thread_args->end_row;
    int thread_id = thread_args->thread_id;
    int validate_rows = thread_args->validate_rows;
    pthread_mutex_t *mutex = thread_args->mutex;

    if (validate_rows) {
        // Validate rows
        for (int i = start_row; i <= end_row; i++) {
            // Implement row validation logic here
        }
        pthread_mutex_lock(mutex);
        printf("Thread ID-%d: Row validation completed.\n", thread_id);
        pthread_mutex_unlock(mutex);
    } else {
        // Validate columns
        // Implement column validation logic here
        pthread_mutex_lock(mutex);
        printf("Thread ID-%d: Column validation completed.\n", thread_id);
        pthread_mutex_unlock(mutex);
    }

    pthread_exit(NULL);
}

void *validateSubGrids(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;
    int (*sudoku)[9] = thread_args->sudoku;
    int thread_id = thread_args->thread_id;
    pthread_mutex_t *mutex = thread_args->mutex;

    // Define sub-grid boundaries based on thread ID
    int start_row, end_row, start_col, end_col;
    switch (thread_id) {
        case 0: // Sub-grids 1, 2, 3
            start_row = 0;
            end_row = 2;
            start_col = 0;
            end_col = 2;
            break;
        case 1: // Sub-grids 4, 5, 6
            start_row = 0;
            end_row = 2;
            start_col = 3;
            end_col = 5;
            break;
        case 2: // Sub-grids 7, 8, 9
            start_row = 0;
            end_row = 2;
            start_col = 6;
            end_col = 8;
            break;
        case 3: // Sub-grids 1, 4, 7
            start_row = 3;
            end_row = 5;
            start_col = 0;
            end_col = 2;
            break;
        case 4: // Sub-grids 2, 5, 8
            start_row = 3;
            end_row = 5;
            start_col = 3;
            end_col = 5;
            break;
        case 5: // Sub-grids 3, 6, 9
            start_row = 3;
            end_row = 5;
            start_col = 6;
            end_col = 8;
            break;
        case 6: // Sub-grids 1, 2, 3
            start_row = 6;
            end_row = 8;
            start_col = 0;
            end_col = 2;
            break;
        case 7: // Sub-grids 4, 5, 6
            start_row = 6;
            end_row = 8;
            start_col = 3;
            end_col = 5;
            break;
        case 8: // Sub-grids 7, 8, 9
            start_row = 6;
            end_row = 8;
            start_col = 6;
            end_col = 8;
            break;
        default:
            // Invalid thread ID
            pthread_mutex_lock(mutex);
            printf("Invalid thread ID for sub-grid validation.\n");
            pthread_mutex_unlock(mutex);
            pthread_exit(NULL);
    }

    // Validate sub-grids
    for (int i = start_row; i <= end_row; i++) {
        for (int j = start_col; j <= end_col; j++) {
            // Implement sub-grid validation logic here
        }
    }

    pthread_mutex_lock(mutex);
    printf("Thread ID-%d: Sub-grid validation completed.\n", thread_id);
    pthread_mutex_unlock(mutex);

    pthread_exit(NULL);
}
