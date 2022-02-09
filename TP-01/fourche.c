#include <stdio.h>
#include <unistd.h> /* sleep(), getpid() */
#include <sys/types.h> /* getpid() */
#include <sys/wait.h> /* wait() */

int main(int argc, char** argv)
{
    /* Récupère le pid du programme */
    int pid = getpid();

    printf("%d: hello world\n", pid);

    /* fork() retourne 0 dans le processus enfant */
    if (fork())
    {
        printf("%d: je suis le parent.\n", pid);
        wait(NULL); /* Attends tout les enfants du père */
    } else 
    {
        pid = getpid(); /* Rafraichit le pid */
        printf("%d: je suis l'enfant.\n", pid);
    }
    return 0;
}
