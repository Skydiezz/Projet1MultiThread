#ifndef SEMAPHORE_STRUCT_H
#define SEMAPHORE_STRUCT_H

#include <stdio.h>

typedef struct {
    volatile int value;      
    volatile int lock_var;   
} semaphore_t;

void semaphore_init_fake(volatile semaphore_t *sem, int init_value);
void sem_wait_fake(volatile semaphore_t *sem);
void sem_post_fake(volatile semaphore_t *sem);

#endif