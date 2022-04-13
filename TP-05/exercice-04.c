#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

sem_t sem_1, sem_2;

void *A(void *args)
{
    for (int i = 0; i < 5; i++)
    {
        sem_wait(&sem_1);
        printf("C1.\n");
        sem_post(&sem_2);
    }
    // while (true) {
    //     sem_wait(&sem_1);
    //     printf("C1.\n");
    //     sem_post(&sem_2);
    // }
    return NULL;
}

void *B(void *args)
{
    for (int i = 0; i < 5; i++) {
        sem_wait(&sem_2);
        printf("C2.\n");
        sem_post(&sem_1);
    }
    // while (true) {
    //     sem_wait(&sem_2);
    //     printf("C2.\n");
    //     sem_post(&sem_1);
    // }
    return NULL;
}

int main()
{
    /* Initialise le semaphore */
    sem_init(&sem_1, 0, 1);
    sem_init(&sem_2, 0, 0); 

    /* Créer les 2 thread A et B */
    pthread_t pA, pB;
    pthread_create(&pA, NULL, A, NULL);
    pthread_create(&pB, NULL, B, NULL);

    /* Attendre que les 2 threads A et B terminent */
    pthread_join(pA, NULL);
    pthread_join(pB, NULL);

    return 0;
}