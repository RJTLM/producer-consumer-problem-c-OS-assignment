//
// OS/Assignment/mssv.c
// Main program for Sudoku Solution Validator
// Created by RyanM on 30/04/2024.
//

#include "mssv.h"
#include "sudoku_validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep

#include "utilities.h"

int sudokuGrid[SUDOKU_SIZE][SUDOKU_SIZE];
int rows[SUDOKU_SIZE];
int columns[SUDOKU_SIZE];
int subgrids[SUDOKU_SIZE];
int counter;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *validate_section(void *args) {
    ThreadArg *arg = (ThreadArg *)args;
    validate_rows(arg->row_start, arg->row_end);
    validate_columns(arg->col_start, arg->col_end);
    validate_subgrids(arg->subgrid_start, arg->subgrid_end);

    // Sleep to simulate delay and show synchronization
    sleep(arg->thread_id);  // Using thread_id as delay for demonstration purposes

    pthread_mutex_lock(&mutex);
    counter += arg->row_end - arg->row_start + 1 + arg->col_end - arg->col_start + 1 + arg->subgrid_end - arg->subgrid_start + 1;
    if (counter == 27) {  // Total 27 validations means this is the last thread
        printf("Thread ID-%d is the last thread.\n", arg->thread_id);
        pthread_cond_signal(&cond);  // Signal parent thread
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

#include <stdio.h>
#include <stdlib.h>

/*int main(int argc, char** argv) {
    printf("Simple Test: 2 + 2 = %d\n", 2 + 2);
    printf("Program has started. Received %d arguments.\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <solution file> <delay>\n", argv[0]);
        return 1;
    }

    printf("Arguments received: %s, %s\n", argv[1], argv[2]);

    // Initialize shared resources
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    counter = 0;

    // Read Sudoku grid from file
    if (!read_sudoku_grid(argv[1])) {
        fprintf(stderr, "Failed to load the Sudoku grid.\n");
        return 1;
    }

    pthread_t threads[4];
    ThreadArg threadArgs[4] = {
        {1, 0, 2, 0, 2, 0, 2},  // Thread 1: First three rows and first three subgrids
        {2, 3, 5, 3, 5, 3, 5},  // Thread 2: Middle three rows and middle three subgrids
        {3, 6, 8, 6, 8, 6, 8},  // Thread 3: Last three rows and last three subgrids
        {4, 0, 8, 0, 8, 0, 8}   // Thread 4: All columns
    };

    // Create threads
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, validate_section, &threadArgs[i]);
    }

    // Wait for the last thread to signal
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    // Join threads
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    // Output results
    printf("Validation Complete. Total valid counts: %d\n", counter);

    // Cleanup
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Program completed\n");
    return 0;
}*/
#include <stdio.h>

int main() {
    printf("Hello, world!\n");
    return 0;
}

