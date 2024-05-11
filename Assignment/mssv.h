//
// OS/Assignment/mssv.h
// Main header for the Sudoku Solution Validator
// Created by RyanM on 30/04/2024.
//

#ifndef MSSV_H
#define MSSV_H

#include <pthread.h>

#define SUDOKU_SIZE 9

extern int sudokuGrid[SUDOKU_SIZE][SUDOKU_SIZE];
extern int rows[SUDOKU_SIZE];
extern int columns[SUDOKU_SIZE];
extern int subgrids[SUDOKU_SIZE];
extern int counter;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;

typedef struct {
    int thread_id;
    int row_start;
    int row_end;
    int col_start;
    int col_end;
    int subgrid_start;
    int subgrid_end;
} ThreadArg;

void *validate_section(void *arg);

#endif // MSSV_H
