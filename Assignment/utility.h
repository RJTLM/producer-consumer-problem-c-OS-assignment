//
// OS/Assignment/utility.h
// Utility functions for the Sudoku Solution Validator
// Created by RyanM on 30/04/2024.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>

// Declare function prototypes
void printErrorMessage(char *message);
void printSuccess(const char *message);
void printFailure(const char *message);
void printInfo(const char *message);
void printSudokuSolution(int sudoku[9][9]);

#endif /* UTILITY_H */
