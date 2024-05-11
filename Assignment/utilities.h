//
// OS/Assignment/utilities.h
// Utility functions for the Sudoku Solution Validator
// Created by RyanM on 30/04/2024.
//

#ifndef UTILITIES_H
#define UTILITIES_H

/**
 * Reads a Sudoku grid from a file.
 * @param filename Path to the file containing the Sudoku grid.
 * @return Pointer to a dynamically allocated 2D array containing the grid.
 */
int** read_sudoku_grid(const char* filename);

/**
 * Frees a dynamically allocated Sudoku grid.
 * @param grid Pointer to the 2D array of the grid to be freed.
 */
void free_sudoku_grid(int** grid);

#endif // UTILITIES_H
