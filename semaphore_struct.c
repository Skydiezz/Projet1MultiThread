#include "h-files/semaphore_struct.h"

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

static void lock(volatile int *locked) {
    while (1) {
        while (*locked == 1);
        if (xchg(locked, 1) == 0) {
            break;
        }
    }
}

static void unlock(volatile int *locked) {
    *locked = 0;
}

void semaphore_init_fake(volatile semaphore_t *sem, int init_value) {
    sem->value = init_value;
    sem->lock_var = 0;
}

void sem_wait_fake(volatile semaphore_t *sem) {
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

void sem_post_fake(volatile semaphore_t *sem) {
    lock(&sem->lock_var);
    sem->value++;
    unlock(&sem->lock_var);
}
