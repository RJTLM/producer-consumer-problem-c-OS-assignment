//
// OS/Assignment/suduko_validator.h
// Header file for Sudoku validation functions
// Created by RyanM on 30/04/2024.
//

#ifndef SUDOKU_VALIDATOR_H
#define SUDOKU_VALIDATOR_H

// Function prototypes for Sudoku validation
void validate_rows(int start, int end);
void validate_columns(int start, int end);
void validate_subgrids(int start, int end);

#endif // SUDOKU_VALIDATOR_H

