#include <pthread.h>
#include "h-files/semaphore_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 8
#define TOTAL_ITEMS 131072


int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;


pthread_mutex_t mutex;
semaphore_t empty;
semaphore_t full;

void insert_item(int item) {
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
}


int remove_item() {
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    return item;
}

void* producer(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < TOTAL_ITEMS / BUFFER_SIZE; i++) {
        sem_wait(&empty); 
        pthread_mutex_lock(&mutex);

        // Section critique 
        insert_item(id);
        printf("Producer %d produced item at index %d\n", id, (in - 1 + BUFFER_SIZE) % BUFFER_SIZE);

        pthread_mutex_unlock(&mutex);
        sem_post(&full); 

        // Simulation du traitement
        for (int j = 0; j < 10000; j++);
    }

    return NULL;
}

void* consumer(void* arg) {

    (void)arg;

    for (int i = 0; i < TOTAL_ITEMS / BUFFER_SIZE; i++) {
        sem_wait(&full); 
        pthread_mutex_lock(&mutex);

        // Section critique 
        remove_item();
        printf("Consumer consumed item from index %d\n", (out - 1 + BUFFER_SIZE) % BUFFER_SIZE);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty); 

        // Simulation du traitement 
        for (int j = 0; j < 10000; j++);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_producers> <num_consumers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);

    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    // Initialisation du mutex et des sémaphores
    pthread_mutex_init(&mutex, NULL);
    semaphore_init(&empty, BUFFER_SIZE);
    semaphore_init(&full, 0);

    // Création des threads producteurs
    for (int i = 0; i < num_producers; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        if (pthread_create(&producers[i], NULL, producer, id) != 0) {
            perror("Failed to create producer thread");
            return EXIT_FAILURE;
        }
    }

    // Création des threads consommateurs
    for (int i = 0; i < num_consumers; i++) {
        if (pthread_create(&consumers[i], NULL, consumer, NULL) != 0) {
            perror("Failed to create consumer thread");
            return EXIT_FAILURE;
        }
    }

    // Attendre les producteurs
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    // Attendre les consommateurs
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Nettoyage
    pthread_mutex_destroy(&mutex);
    semaphore_destroy(&empty);
    semaphore_destroy(&full);

    return EXIT_SUCCESS;
}