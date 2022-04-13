#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem_1, sem_2, sem_3;

void *T1(void *args)
{
    printf("T1.\n");

    sem_post(&sem_1);
    sem_post(&sem_1);
    sem_post(&sem_1);
    return NULL;
}

void *T2(void *args)
{
    sem_wait(&sem_1);
    printf("T2.\n");
    sem_post(&sem_2);
    return NULL;
}

void *T3(void *args)
{
    sem_wait(&sem_1);
    printf("T3.\n");
    sem_post(&sem_2);
    return NULL;
}

void *T4(void *args)
{
    sem_wait(&sem_1);
    printf("T4.\n");
    sem_post(&sem_3);
    return NULL;
}

void *T5(void *args)
{
    sem_wait(&sem_2);
    sem_wait(&sem_2);
    printf("T5.\n");
    sem_post(&sem_3);
    return NULL;
}

void *T6(void *args)
{
    sem_wait(&sem_3);
    printf("T6.\n");
    return NULL;
}

int main()
{
    /* Initialise le semaphore */
    sem_init(&sem_1, 0, 0);
    sem_init(&sem_2, 0, 0); 
    sem_init(&sem_3, 0, 0); 

    /* Créer les 2 thread A et B */
    pthread_t pT1, pT2, pT3, pT4, pT5, pT6;
    pthread_create(&pT1, NULL, T1, NULL);
    pthread_create(&pT2, NULL, T2, NULL);
    pthread_create(&pT3, NULL, T3, NULL);
    pthread_create(&pT4, NULL, T4, NULL);
    pthread_create(&pT5, NULL, T5, NULL);
    pthread_create(&pT6, NULL, T6, NULL);

    /* Attendre que les 2 threads A et B terminent */
    pthread_join(pT1, NULL);
    pthread_join(pT2, NULL);
    pthread_join(pT3, NULL);
    pthread_join(pT4, NULL);
    pthread_join(pT5, NULL);
    pthread_join(pT6, NULL);

    return 0;
}