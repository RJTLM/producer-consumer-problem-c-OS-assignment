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

#define NUM_THREADS 9

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments are provided
    if (argc != 3) {
        printErrorMessage("Usage: ./mssv <solution_file> <delay>");
        exit(EXIT_FAILURE);
    }

    // Extract command-line arguments
    char *filename = argv[1];
    int delay = atoi(argv[2]);

    // Read Sudoku solution from the file
    int sudoku[9][9];
    readSudokuSolution(filename, sudoku);

    // Create thread arguments
    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_args[NUM_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    // Create threads for row validation
    for (int i = 0; i < 3; i++) {
        thread_args[i].sudoku = sudoku;
        thread_args[i].start_row = i * 3;
        thread_args[i].end_row = (i + 1) * 3 - 1;
        thread_args[i].thread_id = i;
        thread_args[i].validate_rows = 1;
        thread_args[i].mutex = &mutex;
    }

    // Create threads for column validation
    for (int i = 3; i < 6; i++) {
        thread_args[i].sudoku = sudoku;
        thread_args[i].start_row = 0;
        thread_args[i].end_row = 8;
        thread_args[i].thread_id = i;
        thread_args[i].validate_rows = 0;
        thread_args[i].mutex = &mutex;
    }

    // Create threads for sub-grid validation
    for (int i = 6; i < NUM_THREADS; i++) {
        thread_args[i].sudoku = sudoku;
        thread_args[i].thread_id = i;
        thread_args[i].mutex = &mutex;
    }

    // Start the timer for delay
    sleep(delay);

    // Create threads and validate rows, columns, and sub-grids
    createThreads(threads, NUM_THREADS, validateRowsAndColumns, (void *)thread_args);

    // Wait for all threads to complete
    joinThreads(threads, NUM_THREADS);

    // Print validation results
    // TODO: Implement printing of validation results

    return 0;
}
