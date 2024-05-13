//
// OS/Assignment/utility.c
// Utility functions for the Sudoku Solution Validatorplease update the header files
// Created by RyanM on 30/04/2024.
//

#include "utility.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void printErrorMessage(char *message) {
    fprintf(stderr, ANSI_COLOR_RED "Error: %s" ANSI_COLOR_RESET "\n", message);
}

void printSuccess(const char *message) {
    printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "\n", message);
}

void printFailure(const char *message) {
    printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", message);
}

void printInfo(const char *message) {
    printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "\n", message);
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

void printMessage(char *message) {
    printf("%s\n", message);
}