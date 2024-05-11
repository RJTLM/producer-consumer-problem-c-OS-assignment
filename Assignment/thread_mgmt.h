//
// OS/Assignment/thread_mgmt.h
// Created by RyanM on 11/05/2024.
//

#ifndef THREAD_MGMT_H
#define THREAD_MGMT_H

#include <pthread.h>
#include "thread_functions.h"  // Include if ThreadArgs is defined here

// Declare function prototypes
void createThreads(pthread_t threads[], int num_threads, void *(*start_routine)(void *), ThreadArgs args_array[]);
void joinThreads(pthread_t threads[], int num_threads);

#endif /* THREAD_MGMT_H */
