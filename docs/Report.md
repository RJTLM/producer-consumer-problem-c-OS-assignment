# COMP2006 Operating Systems - Assignment

## Sudoku Solution Validator (SSV) - Producer Consumer Problem

By Ryan Mackintosh

### Objective

*The objective of this programming assignment is to give you some experiences in using
multiple threads and inter-thread communications. You will learn how to create threads and
solve their critical section problems.*

### 1. Detailed discussion on how synchronization is achieved when accessing shared resources / variables and which threads access the shared resources

In the Sudoku Solution Validator (SSV) program, synchronization is critical to ensure that the shared resources are accessed and modified correctly by multiple threads.
The key shared resources in this program are the `Row`, `Col`, `Sub`, and `Counter` arrays, which store the validation results for rows, columns, and subgrids, as well as the total count of valid rows, columns, and subgrids.

#### Threads and Shared Resources:

- **Parent Thread**:
    - Initialises the shared resources (`Row`, `Col`, `Sub`, and `Counter`).
    - Creates child threads and assigns them specific regions to validate.
    - Waits for child threads to complete their tasks and then displays the final results.

- **Child Threads**:
    - Each child thread validates its assigned rows, columns, and subgrids.
    - Updates the shared resources based on the validation results.

#### Synchronisation Mechanisms:

1. **Mutex Locks**:
    - **pthread_mutex_t shared_memory_mutex**: This mutex is used to control access to the `Row`, `Col`, `Sub`, and `Counter` arrays. 
    - Whenever a child thread needs to update these arrays, it needs to acquire `shared_memory_mutex` lock to ensure that no other thread is modifying the shared resources simultaneously.

   ```c
   pthread_mutex_lock(&shared_memory_mutex);
   // Critical section: update shared resources
   pthread_mutex_unlock(&shared_memory_mutex);
   ```

2. **Condition Variables**:
    - **pthread_cond_t sync_condition**: This condition variable is used for the parent thread to wait for all child threads to complete their validation. 
    - The parent thread blocks on this condition variable until all child threads signal that they have finished their tasks.

   ```c
   pthread_mutex_lock(&thread_counter_mutex);
   if (completed_thread_count < 4) {
       pthread_cond_wait(&sync_condition, &thread_counter_mutex);
   }
   pthread_mutex_unlock(&thread_counter_mutex);
   ```

3. **Thread Counters**:
    - **int completed_thread_count**: This counter keeps track of the number of child threads that have completed their tasks. 
    - Each child thread increments this counter when it finishes its validation. The last thread to finish signals the parent thread to proceed.

   ```c
   pthread_mutex_lock(&thread_counter_mutex);
   completed_thread_count++;
   if (completed_thread_count == 4) {
       pthread_cond_signal(&sync_condition);
   }
   pthread_mutex_unlock(&thread_counter_mutex);
   ```

4. **Thread Queue Management**:
    - **pthread_t thread_queue[]**: This array and its associated mutex `thread_queue_mutex` manage the order in which threads enter the critical section for validation. 
    - This ensures fair access to the critical section and helps prevent thread starvation (being denied resource access due to scheduling).

   ```c
   pthread_mutex_lock(&thread_queue_mutex);
   add_to_queue(pthread_self());
   while (pthread_self() != thread_queue[0]) {
       pthread_mutex_unlock(&thread_queue_mutex);
       // Busy wait or sleep
       pthread_mutex_lock(&thread_queue_mutex);
   }
   pthread_mutex_unlock(&thread_queue_mutex);
   ```

By using these synchronisation mechanisms, the program ensures that all threads can safely access and modify shared resources without causing data corruption or inconsistencies.

### 2. Description of any cases for which your program is not working correctly or how you test your program that makes you believe it works perfectly

#### Testing the Program

The program was tested extensively using multiple Sudoku solution files with known valid and invalid solutions. Here are the steps taken to test the program:

1. **Valid Sudoku Solutions**:
    - Multiple valid Sudoku solution files were created and used to test the program.
    - The program correctly identified these solutions as valid and printed the expected output.

2. **Invalid Sudoku Solutions**:
    - Several invalid Sudoku solution files were created, each containing specific errors such as duplicate numbers in a row, column, or sub-grid.
    - The program correctly identified the invalid rows, columns, and sub-grids, and printed the expected output indicating which parts of the solution were invalid.

3. **Boundary Cases**:
    - Solutions with the minimum and maximum possible delays (1 and 10 seconds) were tested to ensure the program handles these delays correctly.
    - Solutions with varying numbers of invalid entries were tested to ensure the program correctly counts and identifies all invalid rows, columns, and sub-grids.

4. **User Input**:
   - Correct and incorrect command line arguments were tested to ensure sufficient error handling for the purposes of this assignment.

Incorrect Delay Testing:
![DelayTesting](/docs/SampleData/UserInput1.JPG)

Incorrect Filename Testing:
![IncorrectFileNameTesting](/docs/SampleData/UserInput2.JPG)

5. **Edge Cases**:
   - Testing with edge cases such as empty files, files with incorrect formats, or solutions with the maximum number of invalid entries to ensure the program handles all possible scenarios gracefully.

Testing with a file that contains invalid/missing data:
![EdgeCaseTesting](/docs/SampleData/EdgeCase1.JPG)

#### Known Issues

There are no known issues with the program within a Linux (Curtin VMWare) or Windows environment. The screenshots below demonstrate successful Linux terminal operation which is necessary for the assignment. 
The synchronisation mechanisms (mutexes and condition variables) effectively manage access to shared resources, and the program has been tested to handle various scenarios accurately.

However, potential areas for improvement or further testing include:
- **Stress Testing**: Running the program with a very high number of iterations or on systems with different configurations to ensure robustness. I tended to mainly test with a `Delay of 1` because it was time consuming to wait 10 seconds every time a new thread was in the critical section.

In conclusion, based on extensive testing and validation, the program works correctly and meets the requirements of the assignment. 
The synchronisation mechanisms ensure thread-safe access to shared resources, and the program accurately identifies valid and invalid Sudoku solutions.

### 3. Sample inputs and outputs from your running programs. Explain if the outputs are correct / incorrect

#### Valid Solution Input and Output:

![ValidSolutionInput](/docs/SampleData/ValidSolutionInput.JPG)
![ValidSolutionOutput](/docs/SampleData/ValidSolution.JPG)

#### Invalid Solution Input and Output:

![InvalidSolutionInput](/docs/SampleData/InvalidSolutionInput.JPG)
![InvalidSolutionOutput](/docs/SampleData/InvalidSolution.JPG)

### 4. README, Makefile, and Source Code

#### README:

```text
# README

## Sudoku Solution Validator: Producer Consumer Problem

By Ryan Mackintosh

### Directory Structure

OS/
├── Assignment/                 # Directory containing all source files and headers
│   ├── Solutions/                      # Directory containing sample solution files
│   │   ├── solution
│   │   ├── solution1
│   │   ├── solution2
│   │   ├── solution3
│   │   └── solution4
│   │
│   ├── Makefile                        # Makefile for building the project
│   ├── mssv.c                          # Main source file containing the entry point and thread management
│   ├── sudoku_setup.c                  # Memory management for Sudoku Solution Validator
│   ├── sudoku_setup.h                  # Header for memory management
│   ├── sudoku_solution_file_reader.c   # File IO management for reading Sudoku solutions
│   └── sudoku_solution_file_reader.h   # Header for file IO management
│
├── docs/                       # Documentation and additional notes
│   ├── SampleData/                     # Sample data for report
│   │   ├── EdgeCase1.JPG
│   │   ├── InvalidSolution.JPG
│   │   ├── InvalidSolutionInput.JPG
│   │   ├── UserInput1.JPG
│   │   ├── UserInput2.JPG
│   │   ├── ValidSolution.JPG
│   │   └── ValidSolutionInput.JPG
│   │
│   ├── declaration_of_originality.pdf  # Originality declaration
│   └── Report.md                       # Project report
│
└── README.md                   # Instructions on building and running the program

### Build Instructions and Usege

1. Make sure you are in `OS/Assignment` to build.
2. Run `make` to build the project.
3. Run the program with `./mssv Solutions/<solution_file> <delay>`.
   - `<solution_file>` is file name that contains the sudoku e.g. `solution2`
   - `<delay>` is the delay in seconds (must be between 1 and 10).
4. Run `make clean` to remove the compiled files.

    ```
    Run `make clean && make` for convenience and assurance of a clean build every time.
    ```
   
### File Descriptions

- **mssv.c**: Main program file that initializes threads and manages Sudoku validation. 
- **sudoku_setup.c/h**: Functions for initializing and freeing shared memory structures. 
- **sudoku_solution_file_reader.c/h**: Functions for reading Sudoku solutions from files.

### Additional Documentation

- **Report.md**: Contains the project report.
- **declaration_of_originality.pdf**

### Resources

I have used knowledge gained from UCP and OS to write the majority of this program.
Where this is not the case, I have commented the source in the code. 
```

#### Makefile:

```makefile
#
# OS/Makefile
# Makefile for building the Sudoku Solution Validator
# Created by RyanM on 30/04/2024.
#

CC = gcc
CFLAGS = -Wall -pthread -g
SRCS = mssv.c sudoku_solution_file_reader.c sudoku_setup.c
OBJS = $(SRCS:.c=.o)

EXEC = mssv

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJS)
```

#### Source Code:

##### mssv.c:

```c++ (c++ looks prettier than c in this format for display purposes. The code is C.)
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
    int* subGrids; // Array of subgrids to validate
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
        thread_array[i]->solution_info = solution_data;
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
                    printf("subgrid %d", location);
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
        printf("\n\033[31mThe provided sudoku solution is incorrect (only %d rows/columns/subgrids from 27 are valid).\033[0m\n", solution_info->Counter);
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
Purpose: Validates a subgrid in the Sudoku solution.
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
        printf("\n\033[31mRows:\033[0m\n");
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
        printf("\n\033[31mColumns:\033[0m\n");
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
Purpose: Displays the results of subgrid validation.
*/
void show_subgrid_results(SolutionStruct* solution_info) {
    if (solution_info->Counter == 27) {
        printf("\n\033[32mSubgrids: Successful\033[0m\n");
    } else {
        printf("\n\033[31mSubgrids:\033[0m\n");
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

// Why does Ryan wear glasses?

// Because he can't C..
```

##### sudoku_setup.c:

```c++
//
// OS/Assignment/sudoku_setup.c
// Memory Management for Sudoku Solution Validator
// Created by RyanM on 10/05/2024.
//

#include <stdlib.h>
#include "sudoku_setup.h"

/*
Function: initialise_shared_memory
Import: SudokuData* board_info (pointer to the Sudoku board information)
Export: SolutionStruct* (pointer to the initialised solution structure)
Purpose: To initialise the shared memory structures for storing the Sudoku solution and validation results.
*/
// Source: https://github.com/tcbrindle/libsudoku, https://github.com/lordpty/sudoku-solver
SolutionStruct* initialise_shared_memory(SudokuData* board_info) {
    // Allocate memory for the solution structure
    SolutionStruct* solution_info = malloc(sizeof(SolutionStruct));

    // Allocate memory for the 2D array Sol (to store Sudoku solution)
    solution_info->Sol = malloc(board_info->size * sizeof(int*));
    // Allocate memory for Row, Col, and Sub arrays (to store validation results for rows, columns, and sub-grids)
    solution_info->Row = malloc(board_info->size * sizeof(int));
    solution_info->Col = malloc(board_info->size * sizeof(int));
    solution_info->Sub = malloc(board_info->size * sizeof(int));

    solution_info->Counter = 0;

    // Allocate memory for each row in the Sol array
    for (int i = 0; i < board_info->size; i++) {
        solution_info->Sol[i] = malloc(board_info->size * sizeof(int));
    }

    for (int i = 0; i < board_info->size; i++) {
        solution_info->Row[i] = 0;
        solution_info->Col[i] = 0;
        solution_info->Sub[i] = 0;
    }

    return solution_info;
}

/*
Function: free_memory
Import: SolutionStruct* solution_info (pointer to the solution structure),
        SudokuData* board_info (pointer to the Sudoku board information)
Export: void
Purpose: To free the allocated memory for the Sudoku solution and validation results.
*/
// Source: https://github.com/tcbrindle/libsudoku, https://github.com/lordpty/sudoku-solver
void free_memory(SolutionStruct* solution_info, SudokuData* board_info) {
    // Free the memory allocated for each row in the Sol array
    for (int i = 0; i < board_info->size; i++) {
        free(solution_info->Sol[i]);
    }

    // Frees the allocated memory (opposite to function above)
    free(solution_info->Sol);
    free(solution_info->Row);
    free(solution_info->Col);
    free(solution_info->Sub);
    free(board_info);
    free(solution_info); // yay :)
}
```

##### sudoku_setup.h:

```c++
//
// OS/Assignment/sudoku_setup.h
// Memory Management Header File for Sudoku Solution Validator
// Created by RyanM on 10/05/2024.
//

#ifndef SUDOKU_SETUP_H
#define SUDOKU_SETUP_H

// Structure to hold Sudoku board information
typedef struct {
    int size;
    char* sourceFile;
} SudokuData;

// Structure to hold the Sudoku solution and validation results
typedef struct {
    int** Sol;        // 2D array to store the Sudoku solution
    int* Row;         // Array to store the validation results of each row
    int* Col;         // Array to store the validation results of each column
    int* Sub;         // Array to store the validation results of each sub-grid
    int Counter;      // Counter to keep track of the total number of valid rows, columns, and sub-grids
} SolutionStruct;

// Function prototypes for initialising and freeing shared memory
SolutionStruct* initialise_shared_memory(SudokuData* board_info);
void free_memory(SolutionStruct* solution_info, SudokuData* board_info);

#endif
```

##### sudoku_solution_file_reader.c:

```c++
//
// OS/Assignment/sudoku_solution_file_reader.c
// File IO Management for Sudoku Solution Validator
// Created by RyanM on 09/05/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solution_file_reader.h"
#include "sudoku_setup.h"

/*
Function: load_sudoku_from_file
Import: SudokuData* board_data (pointer to the Sudoku board information),
        SolutionStruct* solution_data (pointer to the solution structure)
Export: int (0 for success, 1 for error)
Purpose: To read a Sudoku solution from a file and store it in the provided solution structure.
*/
// Source: https://github.com/tcbrindle/libsudoku, https://github.com/lordpty/sudoku-solver
int load_sudoku_from_file(SudokuData* board_data, SolutionStruct* solution_data) {
    // Open the file containing the Sudoku solution
    FILE* file_pointer = fopen(board_data->sourceFile, "r");
    if (file_pointer == NULL) {
        fprintf(stderr, "\033[31mError\033[0m\t Unable to open the specified file\n");
        return 1;
    }

    for (int row = 0; row < board_data->size; row++) {
        for (int col = 0; col < board_data->size; col++) {
            int num;
            // Read each number from the file (to 'read in' the Sudoku solution)
            if ((fscanf(file_pointer, "%d", &num) != 1) || (num < 1 || num > 9)) {
                fprintf(stderr, "\033[31mError\033[0m\t Invalid data in the file. File must contain integers from 1 to 9 (inclusive) only and in the correct format.\n");
                fclose(file_pointer); // Close the file
                return 1;
            }
            // Store the read number in the solution structure
            solution_data->Sol[row][col] = num;
        }
    }

    fclose(file_pointer);
    return 0;
}
```

##### sudoku_solution_file_reader.h:

```c++
//
// OS/Assignment/sudoku_solution_file_reader.c
// File IO Management Header File for Sudoku Solution Validator
// Created by RyanM on 09/05/2024.
//

#ifndef SUDOKU_SOLUTION_FILE_READER_H
#define SUDOKU_SOLUTION_FILE_READER_H

#include "sudoku_setup.h"

// Function prototype for reading the Sudoku solution from a file
int load_sudoku_from_file(SudokuData* board_data, SolutionStruct* solution_data);

#endif
```
