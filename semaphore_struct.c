#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

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
    while (xchg(locked, 1) == 1) {
    }
}

void unlock(volatile int *locked) {
    *locked = 0;
}

typedef struct {
    int value;             
    volatile int lock_var; 
} semaphore_t;

void semaphore_init_fake(semaphore_t *sem, int init_value) {
    sem->value = init_value;
    sem->lock_var = 0;
}

void sem_wait_fake(semaphore_t *sem) {
    while (1) {
        lock(&sem->lock_var);
        if (sem->value > 0) {
            sem->value--; 
            unlock(&sem->lock_var);
            break;
        }
        unlock(&sem->lock_var);
    }
}

void sem_post_fake(semaphore_t *sem) {
    lock(&sem->lock_var); 
    sem->value++;
    unlock(&sem->lock_var);
}