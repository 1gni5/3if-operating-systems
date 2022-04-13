#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include "bag.h"


// shared variables
int sum;
bag_t *bag;
sem_t mutex;

// each consumer thread runs this function
void *consumer(void *arg)
{
    //int cnum=*(int*)arg;
    //printf("consumer %d: start\n",cnum);
   
    while(1)
    {
        int *box = bb_take(bag);
        assert( box != NULL );

        // Critical section
        sem_wait(&mutex);
        sum = sum + *box;
        sem_post(&mutex);
        
        free(box);
    }

    //printf("consumer %d: end\n",cnum);
    return NULL; // dummy return to comply with required signature
}

// each producer thread runs this function
void *producer(void *arg)
{
    int pnum=*(int*)arg;
    //printf("producer %d:start \n",pnum);

    int k;
    for( k = 0 ; k < pnum+1 ; k++)
    {
        int *box = malloc(sizeof(int));
        assert( box != NULL ); 
        *box = 1;

        bb_add(bag, box);
    }
    
    //printf("producer %d:end\n",pnum);
    return NULL; // dummy return to comply with required signature
}

int main(int argc, char ** argv)
{
    assert(argc == 3);

    int N = atoi( argv[1] );
    assert( N > 0);

    pthread_t prod[N];
    pthread_t cons[N];

    int S = atoi( argv[2] );
    assert( S > 0 );

    // initialize shared variables
    bag=bb_create(S); 
    assert(bag != NULL);
    sum=0;

    // initialize semaphores
    sem_init(&mutex, 0, 1);

    int r;
    for(int pnum = 0 ; pnum < N ; pnum++)
    {
        int *thread_arg=malloc(sizeof(int));
        *thread_arg = pnum;
        r=pthread_create(&prod[pnum], NULL, producer,thread_arg);
        if ( r )
        {
            printf("error: could not spawn producer %d\n",pnum);
            exit(1);
        }
    } 
    for(int cnum = 0 ; cnum < N ; cnum++)
    {
        int *thread_arg=malloc(sizeof(int));
        *thread_arg = cnum;
        r=pthread_create(&cons[cnum],NULL, consumer, thread_arg);
        if ( r )
        {
            printf("error: could not spawn consumer %d\n",cnum);
            exit(1);
        }
    }

    // sleep(2); 

    // Join producer threads
    for(int pnum = 0 ; pnum < N ; pnum++)
    {
        r=pthread_join(prod[pnum], NULL);
        if ( r )
        {
            printf("error: could not join producer %d\n",pnum);
            exit(1);
        }
    }

    printf("theroretical result=%d\n",N*(N+1)/2);
    printf("actual computed sum=%d\n", sum);
    return 0;
}
