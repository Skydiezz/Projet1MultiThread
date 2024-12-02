#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NB_LECTURES 2540
#define NB_ECRITURES 640

int readers = 0;              
pthread_mutex_t mutex;        
pthread_mutex_t write_lock;   

void* lecteur(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < NB_LECTURES; i++) {
        
        pthread_mutex_lock(&mutex);
        readers++;
        if (readers == 1) {  
            pthread_mutex_lock(&write_lock);
        }
        pthread_mutex_unlock(&mutex);

        
        printf("Lecteur %d lit. (%d/2540)\n", id, i + 1);
        for (int i = 0; i < 10000; i++); 

        
        pthread_mutex_lock(&mutex);
        readers--;
        if (readers == 0) {   
            pthread_mutex_unlock(&write_lock);
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* ecrivain(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < NB_ECRITURES; i++) {
        
        pthread_mutex_lock(&write_lock);

        
        printf("Écrivain %d écrit. (%d/640)\n", id, i + 1);
        for (int i = 0; i < 10000; i++); 

        
        pthread_mutex_unlock(&write_lock);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <nombre_lecteurs> <nombre_ecrivains>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int nb_lecteurs = atoi(argv[1]);
    int nb_ecrivains = atoi(argv[2]);

    if (nb_lecteurs <= 0 || nb_ecrivains <= 0) {
        fprintf(stderr, "Le nombre de lecteurs et d'écrivains doit être > 0.\n");
        return EXIT_FAILURE;
    }

    
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&write_lock, NULL);

    pthread_t lecteurs[nb_lecteurs];
    pthread_t ecrivains[nb_ecrivains];

    
    for (int i = 0; i < nb_lecteurs; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        if (pthread_create(&lecteurs[i], NULL, lecteur, id) != 0) {
            perror("Erreur lors de la création d'un thread lecteur");
            return EXIT_FAILURE;
        }
    }

    
    for (int i = 0; i < nb_ecrivains; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        if (pthread_create(&ecrivains[i], NULL, ecrivain, id) != 0) {
            perror("Erreur lors de la création d'un thread écrivain");
            return EXIT_FAILURE;
        }
    }

   
    for (int i = 0; i < nb_lecteurs; i++) {
        if (pthread_join(lecteurs[i], NULL) != 0) {
            perror("Erreur lors de la terminaison d'un thread lecteur");
            return EXIT_FAILURE;
        }
    }

    
    for (int i = 0; i < nb_ecrivains; i++) {
        if (pthread_join(ecrivains[i], NULL) != 0) {
            perror("Erreur lors de la terminaison d'un thread écrivain");
            return EXIT_FAILURE;
        }
    }

   
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_lock);

    printf("Simulation terminée avec succès.\n");
    return EXIT_SUCCESS;
}
