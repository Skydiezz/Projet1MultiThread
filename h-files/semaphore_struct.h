#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <pthread.h>
#include <stdlib.h>

typedef struct {
    volatile unsigned int value;
    pthread_t *waiting_threads;
    volatile int lock_var;
} semaphore_t;

void semaphore_init(semaphore_t *sem, unsigned int init_value);
void semaphore_destroy(semaphore_t *sem);
void sem_wait(semaphore_t *sem);
void sem_post(semaphore_t *sem);

#endif 