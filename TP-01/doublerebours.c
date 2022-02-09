#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    /* Créer deux processus */
    if (fork())
    {
        /* Processus parent */
        execl("./rebours", "./rebours", "5", NULL);
    } else
    {
        /* Processus enfant */
        execl("./rebours", "./rebours", "2", NULL);
    }

    return 0;
}
