#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 8
#define TOTAL_ITEMS 131072
#define ITE 1000000

int N;
sem_t *forke;

void* philo(void* arg){
    int id = *(int*)arg;
    free(arg);

    for(int i = 0; i < ITE; i++){
        printf("Philosophe %d pense.\n", id);
        if(id % 2 == 0){
            sem_wait(&forke[id]);
            sem_wait(&forke[(id + 1) % N]);
        } else {
            sem_wait(&forke[(id + 1) % N]);
            sem_wait(&forke[id]);
        }

        printf("Philosophe %d mange.\n", id);

        sem_post(&forke[id]);
        sem_post(&forke[(id + 1) % N]);
    }
    return NULL;
}


int main(int argc, char* argv[]){

    if(argc != 2){
        fprintf(stderr, "Usage: %s <N>\n",argv[0]);
        return EXIT_FAILURE;
    }

    N = atoi(argv[1]);
    if(N <= 1){
        fprintf(stderr, "Nombre de philo doit ê > 1\n");
        return EXIT_FAILURE;
    }

    forke = malloc(N * sizeof(sem_t));
    for(int i = 0; i < N; i++){
        sem_init(&forke[i], 0, 1);
    }

    pthread_t threads[N];

    for(int i = 0; i < N; i++){
        int* id = malloc(sizeof(int));
        *id = i;
        if(pthread_create(&threads[i], NULL, philo, id) != 0){
            perror("Erreur lors de la création d'un thread");
            return EXIT_FAILURE;
        }
    }

    for(int i = 0; i < N; i++){
        if(pthread_join(threads[i], NULL) != 0){
            perror("Erreur lors de la terminaison d'un thread");
            return EXIT_FAILURE;
        }
    }


    for(int i = 0; i < N; i++){
        sem_destroy(&forke[i]);
    }
    free(forke);

    return EXIT_SUCCESS;


}