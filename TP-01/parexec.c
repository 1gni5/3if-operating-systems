#include <stdio.h>
#include <unistd.h> /* fork() */
#include <assert.h> /* assert() */
#include <sys/wait.h> /* wait() */

int main(int argc, char** argv)
{
    assert(argc > 2); /* Au moins 2 arguments */
    char* prog = argv[1];

    /* Créer un enfant pour chaque arguments */
    for (int i=2; i < argc; i++)
    {
        if (!fork())
        {
            execl(prog, prog, argv[i], NULL);
        }
    }

    /* Attend tout les enfants du père */
    for (int i=2; i < argc; i++) 
    {
        wait(NULL);
    }

    return 0;
}
