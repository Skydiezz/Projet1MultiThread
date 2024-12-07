#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include "h-files/semaphore_struct.h"

// Primitives d'attente active
volatile int locked = 0;

static inline int xchg(volatile int *addr, int newval) {
    int oldval;
    asm volatile (
        "xchg %0, %1"
        : "=r"(oldval), "+m"(*addr)
        : "0"(newval)
        : "memory"
    );
    return oldval;
}

void lock(volatile int *locked) {
    while (1) {
        while (*locked == 1);
        if (xchg(locked, 1) == 0) {
            break;
        }
    }
}

void unlock(volatile int *locked) {
    *locked = 0;
}

void semaphore_init(semaphore_t *sem, unsigned int init_value) {
    sem->value = init_value;
    sem->lock_var = 0;
    sem->waiting_threads = NULL;
}

void semaphore_destroy(semaphore_t *sem) {
    free(sem->waiting_threads);
}

void sem_wait(semaphore_t *sem) {
    lock(&sem->lock_var);

    if (sem->value > 0) {
        sem->value--;
        unlock(&sem->lock_var);
    } else if (sem->value == 0) {
        if (sem->waiting_threads == NULL) {
            sem->waiting_threads = malloc(sizeof(pthread_t));
        } else {
            size_t waiting_count = 0;   
            while (sem->waiting_threads[waiting_count] != 0) {
                waiting_count++;
            }
            sem->waiting_threads = realloc(sem->waiting_threads, sizeof(pthread_t) * (waiting_count + 2));
        }

        size_t i = 0;
        while (sem->waiting_threads[i] != 0) {
            i++;
        }
        sem->waiting_threads[i] = pthread_self();
        sem->waiting_threads[i + 1] = 0; 

        unlock(&sem->lock_var);
    }
}

void sem_post(semaphore_t *sem) {
    lock(&sem->lock_var);

    if (sem->waiting_threads != NULL && sem->waiting_threads[0] != 0) {

        size_t idx = 0;
        while (sem->waiting_threads[idx] != 0) {
            sem->waiting_threads[idx] = sem->waiting_threads[idx + 1];
            idx++;
        }

    } else {
        sem->value++;
    }

    unlock(&sem->lock_var);
}

