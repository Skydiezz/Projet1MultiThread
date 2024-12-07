#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

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

void critical() {
    for (volatile int i = 0; i < 10000; i++); 
}

void *thread_f(void *arg) {
    int sections = *((int *)arg);
    free(arg);

    for (int i = 0; i < sections; i++) {
        lock(&locked);
        critical(); 
        unlock(&locked);
    }

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

    int sections_per_thread = 32768 / num_t;
    pthread_t threads[num_t];


    for (int i = 0; i < num_t; i++) {
        int *sections = malloc(sizeof(int));
        if (!sections) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        *sections = sections_per_thread;

        if (pthread_create(&threads[i], NULL, thread_f, sections) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_t; i++) {
        pthread_join(threads[i], NULL);
    }


    return 0;
}
