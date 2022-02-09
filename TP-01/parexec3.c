#include <stdio.h>
#include <unistd.h> /* fork() */
#include <assert.h> /* assert() */
#include <sys/wait.h> /* wait() */
#include <stdlib.h> /* atoi() */
#include <stdbool.h>

int main(int argc, char** argv)
{
    assert(argc > 3); /* Au moins 3 arguments */
    char* prog = argv[1];

    int nmax = atoi(argv[2]); /* Récupère le nombre de proc max */
    assert(nmax > 0); /* Au moins 1 programme */

    int nproc = 0; /* Nb proc. courant */
    int total = argc - 3; /* Nb proc. total */
    bool stop = false; /* Le programme doit se stopper */

    int* pids = (int*)malloc(total * sizeof(int));

    /* Créer un enfant pour chaque arguments */
    for (int i=3; i < argc; i++)
    {
        /* Limite atteinte, attends qu'un processus se termine. */
        if (nproc >= nmax) {
            int status = 0;

            nproc--; total--;
            wait(&status);
            
            stop = (WIFSIGNALED(status) || WTERMSIG(status));
        }

        if (stop) break;

        /* Créer un nouveau processus */
        if (pids[i - 3] = fork())
        {
            /* Augmente le nb de proc. courant */
            nproc++;
        } else
        {
            execl(prog, prog, argv[i], NULL);
        }
    }

    if (stop) 
    {
        for (int i=0; i < nmax; i++) kill(pids[i], 5); 
    } else 
    {
        /* Attend tout les enfants restants */
        for (int i=0; i < total; i++) wait(NULL);
    }

    return 0;
}
