//
// OS/Assignment/thread_functions.h
// Utility functions for the Sudoku Solution Validator
// Created by RyanM on 11/05/2024.
//

#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

#include <pthread.h>

// Define a struct to pass arguments to thread functions
typedef struct {
    int (*sudoku)[9];  // Pointer to a 9x9 array
    int start_row;
    int end_row;
    int start_col;
    int end_col;
    int thread_id;
    int validate_rows;  // Flag to indicate whether to validate rows (1) or columns (0)
    pthread_mutex_t *mutex;
    int *Row;
    int *Col;
    int *Sub;
    int *Counter;
} ThreadArgs;


// Declare function prototypes
void *validateRowsAndColumns(void *args);
void *validateSubGrids(void *args);

#endif /* THREAD_FUNCTIONS_H */
