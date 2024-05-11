//
// OS/Assignment/file_reader.c
// Implementation of Sudoku validation functions
// Created by RyanM on 30/04/2024.
//

#include "file_reader.h"

void readSudokuSolution(char *filename, int sudoku[9][9]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read the Sudoku solution from the file
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (fscanf(file, "%d", &sudoku[i][j]) != 1) {
                fprintf(stderr, "Error reading Sudoku solution from file: %s\n", filename);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}
