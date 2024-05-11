//
// Created by RyanM on 11/05/2024.
//

#include "thread_mgmt.h"

void createThreads(pthread_t threads[], int num_threads, void *(*start_routine)(void *), void *args) {
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, start_routine, args) != 0) {
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
