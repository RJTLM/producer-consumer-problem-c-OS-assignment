//
// OS/Assignment/mssv.c
// Main program for Sudoku Solution Validator
// Created by RyanM on 30/04/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "sudoku_solution_file_reader.h"
#include "sudoku_setup.h"

// Global variable definitions
// Source: https://github.com/tcbrindle/libsudoku, https://github.com/lordpty/sudoku-solver
pthread_cond_t sync_condition = PTHREAD_COND_INITIALIZER; // Condition variable for thread synchronisation
pthread_mutex_t shared_memory_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for access to shared memory
pthread_mutex_t thread_counter_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for counting finished threads
pthread_mutex_t thread_queue_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for managing the thread queue
int completed_thread_count = 0; // Counter for completed threads
int thread_queue_size = 0; // Size of the thread queue
pthread_t thread_queue[100]; // Queue to hold waiting threads

// Structure to hold thread information
typedef struct {
    int number; // Thread number
    int* rows; // Array of rows to validate
    int* cols; // Array of columns to validate
    int* subGrids; // Array of sub-grids to validate
    SolutionStruct* solution_info; // Shared solution structure
    int sleep_value; // Sleep value for delay
} ThreadStruct;

// Function declarations
void* start_parent_thread(char** argv, int sleep_value);
void signal_parent();
void display_thread_ids();
void add_to_queue(pthread_t thread);
pthread_t remove_from_queue();
void* execute_thread(void* argument);
void validate_row(ThreadStruct* thread_info, int row);
void validate_column(ThreadStruct* thread_info, int col);
void validate_subgrid(ThreadStruct* thread_info, int subgrid);
void display_parent_results(SolutionStruct* solution_info, pthread_t* threads);
void display_final_verdict(SolutionStruct* solution_info);
void show_row_results(SolutionStruct* solution_info);
void show_column_results(SolutionStruct* solution_info);
void show_subgrid_results(SolutionStruct* solution_info);

int main(int argc, char** argv) {
    /*
    Function: main
    Import: int argc, char** argv
    Export: int
    Purpose: Entry point of the program. Validates command line arguments and starts the parent thread.
    */
    if (argc != 3) {
        fprintf(stderr, "\033[31mError\033[0m\tInvalid number of command line arguments (try './mssv solution 1')\n");
        exit(1);
    }
    if (argv[1] == NULL) {
        fprintf(stderr, "\033[31mError\033[0m\tNo or invalid solution file provided (try './mssv solution 1' as your CLA)\n");
        exit(1);
    }

    int delay = atoi(argv[2]);
    if (delay > 10 || delay < 1) {
        fprintf(stderr, "\033[31mInvalid Delay\033[0m\t(Must be 1 to 10 inclusive (try './mssv solution 1' as your CLA)\n");
        exit(1);
    }

    int sleep_value = atoi(argv[2]);
    start_parent_thread(argv, sleep_value);
    return 0;
}

/*
Function: start_parent_thread
Import: char** argv, int sleep_value
Export: void*
Purpose: Initialises the Sudoku data, creates child threads for validation, and waits for them to complete.
*/
void* start_parent_thread(char** argv, int sleep_value) {
    SudokuData* board_data = malloc(sizeof(SudokuData));
    board_data->sourceFile = argv[1];
    board_data->size = 9;

    SolutionStruct* solution_data = initialise_shared_memory(board_data);

    if (load_sudoku_from_file(board_data, solution_data) != 0) {
        free_memory(solution_data, board_data);
        exit(1);
    }

    pthread_t threads[4];
    ThreadStruct** thread_array = malloc(sizeof(ThreadStruct*) * 4);
    for (int i = 0; i < 4; i++) {
        thread_array[i] = malloc(sizeof(ThreadStruct));
        thread_array[i]->solution_info = solution_data; // Corrected member name
        thread_array[i]->number = i + 1;
        thread_array[i]->sleep_value = sleep_value;

        if (i < 3) {
            thread_array[i]->rows = malloc(sizeof(int) * 3);
            thread_array[i]->subGrids = malloc(sizeof(int) * 3);
            for (int j = 0; j < 3; j++) {
                thread_array[i]->rows[j] = (i * 3) + j;
                thread_array[i]->subGrids[j] = (i * 3) + j + 1;
            }
            thread_array[i]->cols = NULL;
            pthread_create(&threads[i], NULL, execute_thread, (void*)thread_array[i]);
        } else {
            thread_array[i]->cols = malloc(sizeof(int) * 9);
            for (int j = 0; j < 9; j++) {
                thread_array[i]->cols[j] = j;
            }
            thread_array[i]->rows = NULL;
            thread_array[i]->subGrids = NULL;
            pthread_create(&threads[i], NULL, execute_thread, (void*)thread_array[i]);
        }
    }

    pthread_mutex_lock(&thread_counter_mutex);
    if (completed_thread_count < 4) {
        pthread_cond_wait(&sync_condition, &thread_counter_mutex);
    }
    pthread_mutex_unlock(&thread_counter_mutex);

    display_parent_results(solution_data, threads);
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&shared_memory_mutex);
    pthread_mutex_destroy(&thread_queue_mutex);
    pthread_mutex_destroy(&thread_counter_mutex);
    pthread_cond_destroy(&sync_condition);

    for (int i = 0; i < 4; i++) {
        if (i < 3) {
            free(thread_array[i]->rows);
            free(thread_array[i]->subGrids);
        } else {
            free(thread_array[i]->cols);
        }
        free(thread_array[i]);
    }
    free(thread_array);
    free_memory(solution_data, board_data);
    return NULL;
}

/*
Function: signal_parent
Import: None
Export: void
Purpose: Signals the parent thread that a child thread has completed its work.
*/
void signal_parent() {
    pthread_mutex_lock(&thread_counter_mutex);
    completed_thread_count++;
    if (completed_thread_count == 4) {
        pthread_t thread_id = pthread_self();
        printf("\n\033[33mThread ID-%lu is the last thread.\033[0m\n", (unsigned long) thread_id);
        pthread_cond_signal(&sync_condition);
    }
    pthread_mutex_unlock(&thread_counter_mutex);
}

/*
Function: display_thread_ids
Import: None
Export: void
Purpose: Displays the IDs of threads in the waiting queue.
*/
void display_thread_ids() {
    if (thread_queue_size == 0) {
        printf("\tWaiting queue is empty.\n");
        return;
    }

    printf("Threads waiting in queue:\n");
    for (int i = 0; i < thread_queue_size; i++) {
        printf("Thread : %lu\n", (unsigned long) thread_queue[i]);
    }
}

/*
Function: add_to_queue
Import: pthread_t thread
Export: void
Purpose: Adds a thread to the waiting queue.
*/
void add_to_queue(pthread_t thread) {
    pthread_mutex_lock(&thread_queue_mutex);
    thread_queue[thread_queue_size++] = thread;
    pthread_mutex_unlock(&thread_queue_mutex);
}

/*
Function: remove_from_queue
Import: None
Export: pthread_t
Purpose: Removes and returns the first thread from the waiting queue.
*/
pthread_t remove_from_queue() {
    pthread_mutex_lock(&thread_queue_mutex);
    pthread_t thread_id = thread_queue[0];
    for (int i = 1; i < thread_queue_size; i++) {
        thread_queue[i - 1] = thread_queue[i];
    }
    thread_queue_size--;
    pthread_mutex_unlock(&thread_queue_mutex);

    return thread_id;
}

/*
Function: display_final_verdict
Import: SolutionStruct* solution_info
Export: void
Purpose: Displays the final verdict of the Sudoku validation.
*/
void display_final_verdict(SolutionStruct* solution_info) {
    if (solution_info->Counter != 27) {
        show_row_results(solution_info);
        show_column_results(solution_info);
        show_subgrid_results(solution_info);
    } else {
        printf("\n\033[32mThe provided sudoku solution is correct.\033[0m\n");
    }
}

/*
Function: display_parent_results
Import: SolutionStruct* solution_info, pthread_t* threads
Export: void
Purpose: Displays the results of validation from all threads.
*/
void display_parent_results(SolutionStruct* solution_info, pthread_t* threads) {
    display_final_verdict(solution_info);
    printf("\n");
    for (int i = 0; i < 4; i++) {
        int location, plural, found_problem;
        plural = 0;
        found_problem = 0;
        printf("Thread ID-%lu: ", (unsigned long) threads[i]);
        if (i < 3 && solution_info->Counter != 27) {
            for (int j = 0; j < 3; j++) {
                location = (i * 3) + j;
                if (solution_info->Row[location] == 0) {
                    if (found_problem > 0) {
                        printf(", ");
                    }
                    printf("row %d", location + 1);
                    plural++;
                    found_problem++;
                }
            }
            for (int j = 0; j < 3; j++) {
                location = (i * 3) + j + 1;
                if (solution_info->Sub[location - 1] == 0) {
                    if (found_problem > 0) {
                        printf(", ");
                    }
                    printf("sub-grid %d", location);
                    plural++;
                    found_problem++;
                }
            }
        } else if (i == 3 && solution_info->Counter != 27) {
            for (int j = 0; j < 9; j++) {
                if (solution_info->Col[j] == 0) {
                    if (found_problem > 0) {
                        printf(", ");
                    }
                    printf("column %d", j + 1);
                    plural++;
                    found_problem++;
                }
            }
        }
        if (found_problem > 0) {
            if (plural > 1) {
                printf(" are");
            } else {
                printf(" is");
            }
            printf(" invalid\n");
        } else {
            printf("is valid\n");
        }
    }
    if (solution_info->Counter == 27) {
        printf("\n\033[32mThe provided sudoku solution is correct.\033[0m\n");
    } else {
        printf("\n\033[31mThe provided sudoku solution is incorrect (only %d rows/columns/subgrids are valid).\033[0m\n", solution_info->Counter);
    }
}

/*
Function: validate_row
Import: ThreadStruct* thread_info, int row
Export: void
Purpose: Validates a row in the Sudoku solution.
*/
void validate_row(ThreadStruct* thread_info, int row) {
    int values[9];
    for (int i = 0; i < 9; i++) {
        values[i] = thread_info->solution_info->Sol[row][i];
    }

    pthread_t thread_id = pthread_self();

    pthread_mutex_lock(&shared_memory_mutex);
    remove_from_queue();

    int is_valid = 1;
    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 9; j++) {
            if (values[i] == values[j] || values[i] < 1 || values[i] > 9) {
                is_valid = 0;
                break;
            }
        }
    }
    if (is_valid) {
        thread_info->solution_info->Row[row] = 1;
        thread_info->solution_info->Counter++;
    }
    printf("\n\033[33mThread in critical section:\nThread   %lu\033[0m\n", (unsigned long) thread_id);
    display_thread_ids();
    usleep(thread_info->sleep_value * 500000);

    pthread_mutex_unlock(&shared_memory_mutex);
}

/*
Function: validate_subgrid
Import: ThreadStruct* thread_info, int subgrid
Export: void
Purpose: Validates a sub-grid in the Sudoku solution.
*/
void validate_subgrid(ThreadStruct* thread_info, int subgrid) {
    int row_start, row_stop, col_start, col_stop;

    int grid = subgrid - 1;
    row_start = ((grid) / 3) * 3;
    row_stop = row_start + 2;

    col_start = ((grid) % 3) * 3;
    col_stop = col_start + 2;

    int values[9];
    int index = 0;
    for (int i = row_start; i <= row_stop; i++) {
        for (int j = col_start; j <= col_stop; j++) {
            values[index++] = thread_info->solution_info->Sol[i][j];
        }
    }
    pthread_t thread_id = pthread_self();

    pthread_mutex_lock(&shared_memory_mutex);
    remove_from_queue();
    int is_valid = 1;
    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 9; j++) {
            if (values[i] == values[j] || values[i] < 1 || values[i] > 9) {
                is_valid = 0;
                break;
            }
        }
    }
    if (is_valid) {
        thread_info->solution_info->Sub[grid] = 1;
        thread_info->solution_info->Counter++;
    }
    printf("\n\033[33mThread in critical section:\nThread   %lu\033[0m\n", (unsigned long) thread_id);
    display_thread_ids();
    usleep(thread_info->sleep_value * 500000);
    pthread_mutex_unlock(&shared_memory_mutex);
}

/*
Function: validate_column
Import: ThreadStruct* thread_info, int col
Export: void
Purpose: Validates a column in the Sudoku solution.
*/
void validate_column(ThreadStruct* thread_info, int col) {
    int values[9];
    for (int i = 0; i < 9; i++) {
        values[i] = thread_info->solution_info->Sol[i][col];
    }
    pthread_t thread_id = pthread_self();
    pthread_mutex_lock(&shared_memory_mutex);
    remove_from_queue();
    int is_valid = 1;
    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 9; j++) {
            if (values[i] == values[j] || values[i] < 1 || values[i] > 9) {
                is_valid = 0;
                break;
            }
        }
    }
    if (is_valid) {
        thread_info->solution_info->Col[col] = 1;
        thread_info->solution_info->Counter++;
    }
    printf("\n\033[33mThread in critical section:\nThread   %lu\033[0m\n", (unsigned long) thread_id);
    display_thread_ids();
    usleep(thread_info->sleep_value * 500000);
    pthread_mutex_unlock(&shared_memory_mutex);
}

/*
Function: execute_thread
Import: void* argument
Export: void*
Purpose: Entry point for each thread. Performs validation of assigned regions.
*/
void* execute_thread(void* argument) {
    ThreadStruct* thread_info = (ThreadStruct*)argument;
    if (thread_info->number < 4) {
        for (int i = 0; i < 6; i++) {
            add_to_queue(pthread_self());
            while (1) {
                pthread_mutex_lock(&thread_queue_mutex);
                if (pthread_self() == thread_queue[0]) {
                    pthread_mutex_unlock(&thread_queue_mutex);
                    break;
                }
                pthread_mutex_unlock(&thread_queue_mutex);
            }
            if (i < 3) {
                if (i % 2 == 0) {
                    validate_row(thread_info, thread_info->rows[i]);
                } else {
                    validate_subgrid(thread_info, thread_info->subGrids[i]);
                }
            } else {
                if ((i - 3) % 2 == 0) {
                    validate_subgrid(thread_info, thread_info->subGrids[i - 3]);
                } else {
                    validate_row(thread_info, thread_info->rows[i - 3]);
                }
            }
        }
    } else {
        for (int i = 0; i < 9; i++) {
            add_to_queue(pthread_self());
            while (1) {
                pthread_mutex_lock(&thread_queue_mutex);
                if (pthread_self() == thread_queue[0]) {
                    pthread_mutex_unlock(&thread_queue_mutex);
                    break;
                }
                pthread_mutex_unlock(&thread_queue_mutex);
            }
            validate_column(thread_info, thread_info->cols[i]);
        }
    }
    signal_parent();
    return NULL;
}

/*
Function: show_row_results
Import: SolutionStruct* solution_info
Export: void
Purpose: Displays the results of row validation.
*/
void show_row_results(SolutionStruct* solution_info) {
    if (solution_info->Counter == 27) {
        printf("\n\033[32mRows: Successful\033[0m\n");
    } else {
        printf("\n\033[31mRows: Unsuccessful\033[0m\n");
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (solution_info->Row[i] == 0) {
                    printf("\033[31m");
                }
                printf(" %d \033[0m", solution_info->Sol[i][j]);
            }
            printf("\n");
        }
    }
}

/*
Function: show_column_results
Import: SolutionStruct* solution_info
Export: void
Purpose: Displays the results of column validation.
*/
void show_column_results(SolutionStruct* solution_info) {
    if (solution_info->Counter == 27) {
        printf("\n\033[32mColumns: Successful\033[0m\n");
    } else {
        printf("\n\033[31mColumns: Unsuccessful\033[0m\n");
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (solution_info->Col[j] == 0) {
                    printf("\033[31m");
                }
                printf(" %d \033[0m", solution_info->Sol[i][j]);
            }
            printf("\n");
        }
    }
}

/*
Function: show_subgrid_results
Import: SolutionStruct* solution_info
Export: void
Purpose: Displays the results of sub-grid validation.
*/
void show_subgrid_results(SolutionStruct* solution_info) {
    if (solution_info->Counter == 27) {
        printf("\n\033[32mSubgrids: Successful\033[0m\n");
    } else {
        printf("\n\033[31mSubgrids: Unsuccessful\033[0m\n");
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                int subgrid_row = i / 3;
                int subgrid_col = j / 3;
                int subgrid_index = subgrid_row * 3 + subgrid_col;
                if (solution_info->Sub[subgrid_index] == 0) {
                    printf("\033[31m");
                }
                printf(" %d \033[0m", solution_info->Sol[i][j]);
            }
            printf("\n");
        }
    }
}
