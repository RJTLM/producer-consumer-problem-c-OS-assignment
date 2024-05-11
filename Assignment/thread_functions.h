//
// OS/Assignment/thread_functions.h
// Utility functions for the Sudoku Solution Validator
// Created by RyanM on 11/05/2024.
//

#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

// Define any constants or global variables here

// Declare function prototypes
void *validateRowsColumns(void *args);
void *validateSubGrids(void *args);

#endif /* THREAD_FUNCTIONS_H */

