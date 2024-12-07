#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "h-files/semaphore_struct.h"


int cmp_red = 0;       
int cmp_leci = 0;      
semaphore_t mutex_cmp_lec;   
semaphore_t mutex_cmp_ecr;  
semaphore_t critique;           
semaphore_t queue;              


void rienfaire() {
    for (int i = 0; i < 10000; i++);
}

void *lec(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < 2540; i++) {
        sem_wait(&queue);          
        sem_wait(&mutex_cmp_lec);
        if (cmp_red == 0) {
            sem_wait(&critique);   
        }
        cmp_red++;
        sem_post(&mutex_cmp_lec);
        sem_post(&queue);         

        
        printf("Lecteur %d lit les données.\n", id);
        rienfaire();

        sem_wait(&mutex_cmp_lec);
        cmp_red--;
        if (cmp_red == 0) {
            sem_post(&critique);
        }
        sem_post(&mutex_cmp_lec);
    }
    return NULL;
}


void *writer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < 640; i++) {
        sem_wait(&mutex_cmp_ecr);
        cmp_leci++;
        if (cmp_leci == 1) {
            sem_wait(&queue);     
        }
        sem_post(&mutex_cmp_ecr);

        sem_wait(&critique);       
        
        printf("Écrivain %d écrit des données.\n", id);
        rienfaire();
        sem_post(&critique);

        sem_wait(&mutex_cmp_ecr);
        cmp_leci--;
        if (cmp_leci == 0) {
            sem_post(&queue);      
        }
        sem_post(&mutex_cmp_ecr);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <nombre_lecteurs> <nombre_lecivains>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int nb_lec = atoi(argv[1]);
    int nb_ecr = atoi(argv[2]);

    pthread_t lecs[nb_lec];
    pthread_t ecr[nb_ecr];
    int ids_lec[nb_lec];
    int ids_ecr[nb_ecr];

    semaphore_init(&mutex_cmp_lec, 1);
    semaphore_init(&mutex_cmp_ecr, 1);
    semaphore_init(&critique, 1);
    semaphore_init(&queue, 1);

    for (int i = 0; i < nb_lec; i++) {
        ids_lec[i] = i + 1;
        pthread_create(&lecs[i], NULL, lec, &ids_lec[i]);
    }


    for (int i = 0; i < nb_ecr; i++) {
        ids_ecr[i] = i + 1;
        pthread_create(&ecr[i], NULL, writer, &ids_ecr[i]);
    }

    for (int i = 0; i < nb_lec; i++) {
        pthread_join(lecs[i], NULL);
    }


    for (int i = 0; i < nb_ecr; i++) {
        pthread_join(ecr[i], NULL);
    }


    semaphore_destroy(&mutex_cmp_lec);
    semaphore_destroy(&mutex_cmp_ecr);
    semaphore_destroy(&critique);
    semaphore_destroy(&queue);

    printf("Programme terminé.\n");
    return EXIT_SUCCESS;
}