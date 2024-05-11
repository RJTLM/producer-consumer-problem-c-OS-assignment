//
// OS/Assignment/thread_mgmt.c
// Created by RyanM on 11/05/2024.
//

/*
 * This module manages the lifecycle of threads. It includes functions to create and join threads.
 * Essentially, it abstracts the boilerplate operations associated with thread management,
 * such as starting up threads and ensuring they all complete their execution properly before the
 * program proceeds or terminates. This helps keep the main application logic clean and focused,
 * separating concerns between "doing the work" (handled by thread_functions)
 * and "managing the workers" (handled by thread_mgmt).
 */
#include "thread_mgmt.h"
#include "thread_functions.h"

void createThreads(pthread_t threads[], int num_threads, void *(*start_routine)(void *), ThreadArgs args_array[]) {
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, start_routine, &args_array[i]) != 0) {
            printErrorMessage("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }
}

void joinThreads(pthread_t threads[], int num_threads) {
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printErrorMessage("Failed to join thread");
            exit(EXIT_FAILURE);
        }
    }
}
