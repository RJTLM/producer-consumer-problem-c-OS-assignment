//
// OS/Assignment/mssv.c
// Main program for Sudoku Solution Validator
// Created by RyanM on 30/04/2024.
//

#include "mssv.h"
#include "file_reader.h"
#include "thread_functions.h"
#include "utility.h"
#include "thread_mgmt.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <solution_file> <delay>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    int delay = atoi(argv[2]);

    validateSudokuSolution(filename, delay);

    return 0;
}
