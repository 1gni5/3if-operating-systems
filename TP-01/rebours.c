#include <stdio.h>
#include <stdlib.h> /* atoi() */
#include <unistd.h> /* sleep(), getpid() */
#include <sys/types.h> /* getpid() (pas obligatoire?) */
#include <assert.h> /* assert() */

int main(int argc, char** argv)
{
    assert(argc > 1); /* Au moins un argument */

    /* Récupère et convertit le temps */
    int time = atoi(argv[1]);
    assert(time > 0); /* Compteur positif */
    
    /* Récupère le pid du programme */
    int pid = getpid();

    printf("%d: debut\n", pid);
    for (int i=0; i < time; i++)
    {
        printf("%d: %d\n", pid, time - i);
        sleep(1); /* Attends 1 seconde */
    }
    printf("%d: fin\n", pid);

    return 0;
}
