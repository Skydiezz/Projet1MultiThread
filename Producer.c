#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 8
#define TOTAL_ITEMS 131072

// Buffer partagé
int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

// Mutex et variables de condition
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

// Nombre d'éléments restants à produire/consommer
int items_left = TOTAL_ITEMS;

// Fonction du thread producteur
void* producer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        // Attendre une place disponible
        while (count == BUFFER_SIZE && items_left > 0) {
            pthread_cond_wait(&not_full, &mutex);
        }

        if (items_left <= 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Production
        buffer[in] = id;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        items_left--;

        pthread_cond_signal(&not_empty); // Réveiller un consommateur
        pthread_mutex_unlock(&mutex);

        // Simuler un traitement
        for (int i = 0; i < 10000; i++);
    }

    free(arg);
    return NULL;
}

// Fonction du thread consommateur
void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // Attendre un élément disponible
        while (count == 0 && items_left > 0) {
            pthread_cond_wait(&not_empty, &mutex);
        }

        if (items_left <= 0 && count == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Consommation
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full); // Réveiller un producteur
        pthread_mutex_unlock(&mutex);

        // Simuler un traitement
        for (int i = 0; i < 10000; i++);

        // Affichage de l'élément consommé (facultatif)
        printf("Consumer consumed: %d\n", item);
    }

    return NULL;
}

// Programme principal
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_producers> <num_consumers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);

    pthread_t producers[num_producers], consumers[num_consumers];

    // Créer les threads producteurs
    for (int i = 0; i < num_producers; i++) {
        int* id = malloc(sizeof(int)); // Allouer dynamiquement pour éviter les conflits
        *id = i + 1;
        pthread_create(&producers[i], NULL, producer, id);
    }

    // Créer les threads consommateurs
    for (int i = 0; i < num_consumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    // Attendre les producteurs
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    // Attendre les consommateurs
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Nettoyer
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return EXIT_SUCCESS;
}
