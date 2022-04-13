#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void *thread ( void *vargp){ while(1) sleep(1); }
  
int main()
{
    int err = 0, count = 0;

    while (err == 0)
    {
        pthread_t tid;
        err = pthread_create (&tid, NULL, thread, NULL);
        count++;
    }
    printf("Successfully created %d threads\n", count);
}
