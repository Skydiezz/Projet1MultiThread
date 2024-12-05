#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

void critical(int thread_id) {
    printf("Thread %d is entering the critical section.\n", thread_id);
    for (volatile int i = 0; i < 1000000; i++);
    printf("Thread %d is leaving the critical section.\n", thread_id);
}


void *thread_f(void *arg) {
    int thread_id = *((int *)arg);
    free(arg);

    lock(&locked);
    critical(thread_id);
    unlock(&locked);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_t = atoi(argv[1]);
    if (num_t <= 0) {
        fprintf(stderr, "Number of threads must be positive.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[num_t];

    for (int i = 0; i < num_t; i++) {
        int *thread_id = malloc(sizeof(int));
        if (!thread_id) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        *thread_id = i;
        if (pthread_create(&threads[i], NULL, thread_f, thread_id) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_t; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have finished execution.\n");
    return 0;
}
