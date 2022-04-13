#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include "bag.h"


// Shared variables
int sum;
bag_t *bag;
sem_t mutex;

// Function used by every consumer threads
void *consumer(void *arg)
{
    int cnum=*(int*)arg;
    printf("consumer %d: start\n",cnum);
   
    while(1)
    {
        int *box = bb_take(bag);
        // assert( box != NULL );
        if (box == NULL) 
        {
            printf("consumer %d: end\n",cnum);
            break;
        }

        // Critical section
        sem_wait(&mutex);
        sum = sum + *box;
        sem_post(&mutex);
        
        free(box);
    }

    printf("consumer %d: end\n",cnum);
    return NULL; // Required by signature
}

// Function used by every producer threads
void *producer(void *arg)
{
    int pnum=*(int*)arg;
    printf("producer %d:start \n",pnum);

    for(int k = 0 ; k < pnum+1; k++)
    {
        int *box = malloc(sizeof(int));
        assert( box != NULL ); 
        *box = 1;

        bb_add(bag, box);
    }
    
    printf("producer %d:end\n",pnum);
    return NULL; // Required by signature
}

int main(int argc, char ** argv)
{
    // Should have 3 arguments:
    assert(argc == 3);

    // Number of consumers and producers threads
    int N = atoi( argv[1] );
    assert( N > 0);

    pthread_t prod[N];
    pthread_t cons[N];

    // Total size of the bag
    int S = atoi( argv[2] );
    assert( S > 0 );

    // Initialize shared variables
    bag = bb_create(S); 
    assert(bag != NULL);
    sum = 0;

    // Initialize semaphores
    sem_init(&mutex, 0, 1);

    for(int pnum = 0 ; pnum < N ; pnum++)
    {
        int *thread_arg=malloc(sizeof(int));
        *thread_arg = pnum;
        int r = pthread_create(&prod[pnum], NULL, producer,thread_arg);
        if (r != 0)
        {
            printf("error: could not spawn producer %d\n",pnum);
            exit(1);
        }
    }

    for(int cnum = 0 ; cnum < N ; cnum++)
    {
        int *thread_arg=malloc(sizeof(int));
        *thread_arg = cnum;
        int r = pthread_create(&cons[cnum],NULL, consumer, thread_arg);
        if (r != 0)
        {
            printf("error: could not spawn consumer %d\n",cnum);
            exit(1);
        }
    }



    // Join producer threads
    for(int pnum = 0 ; pnum < N ; pnum++)
    {
        int r = pthread_join(prod[pnum], NULL);
        if ( r != 0 )
        {
            printf("error: could not join producer %d\n",pnum);
            exit(1);
        }
    }

    // Ferme le sac
    bb_close(bag, N);

    // Join consumer threads
    for(int cnum = 0 ; cnum < N ; cnum++)
    {
        int r = pthread_join(cons[cnum], NULL);
        if (r != 0)
        {
            printf("error: could not join consumer %d\n",cnum);
            exit(1);
        }
    }

    printf("Theroretical result=%d\n", N * (N + 1) / 2);
    printf("Actual computed sum=%d\n", sum);

    return 0;
}
