#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem;

void *A(void *args)
{
    printf("C1.\n");
    sem_post(&sem);
    return NULL;
}

void *B(void *args)
{
    sem_wait(&sem);
    printf("C2.\n");
    return NULL;
}

int main()
{
    /* Initialise le semaphore */
    sem_init(&sem, 0, 0); // Initialise le semaphore à 0

    /* Créer les 2 thread A et B */
    pthread_t pA, pB;
    pthread_create(&pA, NULL, A, NULL);
    pthread_create(&pB, NULL, B, NULL);

    /* Attendre que les 2 threads A et B terminent */
    pthread_join(pA, NULL);
    pthread_join(pB, NULL);

    return 0;
}