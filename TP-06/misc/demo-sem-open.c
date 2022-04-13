#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h> 

sem_t *S1;
sem_t *S2;
pthread_t A;
pthread_t B;

// demo: changing these will break the C1-C2-C1... alternation
#define S1_INITIAL_VALUE 0
#define S2_INITIAL_VALUE 0

void* fonctionA(void*data){
    while(1){
        usleep(random() % (int)1e6);
        printf("C1\n");
        sem_post(S1);
        sem_wait(S2);
    }
}

void* fonctionB(void*data){
    sem_wait(S1);
    while(1){
        usleep(random() % (int)1e6);
        printf("  C2\n");
        sem_post(S2);
        sem_wait(S1);
    }
}

int main()
{
    int r;
    
#if defined(__linux__)
    printf("Linux: sem_init\n");
    // In POSIX we can use anonymous semaphores which exist within the
    // process. The "malloc"  is just to match the  pointer types used
    // in  the  macOS version,  but  of  course  each sem_t  could  be
    // declared as scalar and initialized with e.g. sem_init(&S1...)
    S1=malloc(sizeof(sem_t));
    assert(S1 != NULL);
    S2=malloc(sizeof(sem_t));
    assert(S2 != NULL);
    r=sem_init(S1, 0, S1_INITIAL_VALUE);
    assert(r==0);
    r=sem_init(S1, 0, S2_INITIAL_VALUE);
    assert(r==0);
#elif defined(__APPLE__)
    printf("macOS: sem_open+sem_unlink\n");
    // On MacOS there are no anonymous semaphores, so we have to use a
    // different API. the call to  `unlink` is required, otherwise the
    // semas will live  longer than our process and  eventually end up
    // with  arbitrary  values  in  later  executions.  (O_EXCL  helps
    // detecting such stray semas)
    S1=sem_open("/S1",O_CREAT|O_EXCL,0644,  S1_INITIAL_VALUE);
    assert(S1 != SEM_FAILED);
    S2=sem_open("/S2",O_CREAT|O_EXCL,0644,  S2_INITIAL_VALUE);
    assert(S2 != SEM_FAILED);
    r=sem_unlink("/S1");
    assert(r==0);
    r=sem_unlink("/S2");
    assert(r==0);
#else
    #error Unknown platform
#endif
    
    pthread_create(&A,NULL,fonctionA,NULL);
    pthread_create(&B,NULL,fonctionB,NULL);

    sleep(10);
}
