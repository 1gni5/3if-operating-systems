#include <stdio.h>
#include <unistd.h> /* sleep(), getpid() */
#include <sys/types.h> /* getpid() */
#include <sys/wait.h> /* wait() */

int main(int argc, char** argv)
{
    /* Illustration de l'exercice page 3 */
    fork(); /* 2 Processus */
    if (fork()) /* 4 Processus */
    {
        fork(); /* 6 Processus */
    }

    printf("A\n");
    return 0;
}