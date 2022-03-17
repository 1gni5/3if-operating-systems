#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h> /* wait() */
#include <unistd.h> /* fork() */

void afficher(char* crible, int n)
{
    // TODO: afficher les nombres premiers plus petits que n
    printf("Nombre premier:");
    for (int i = 2; i < n; i++)
    {
        if (crible[i] == 1)
        {
            printf(" %d", i);
        }
    }
    printf("\n");
}

void rayer_multiples(char* crible, int n, int k)
{
    // TODO: rayer (i.e. passer à zéro) tous les multiples de k qui sont plus petits que n
    for (int i = 2; i * k < n; i++)
    {
        crible[i * k] = 0;
    }
    
}

int main(int argc, char **argv)
{
    int n=100;
    if(argc>1)
    {
        n = atoi(argv[1]);
        assert( n > 0 );
    }

    // Pour l'exercice 1
    // char buffer[1000];
    // assert(n <= 1000);
    // char * crible = (char*)&buffer ;
    
    // Pour l'exercice 2
    // char* crible = (char*)mmap(NULL, n, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);
    char* crible = (char*)mmap(NULL, n, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, 0, 0);
    
    // Test si l'allocation s'est bien déroulée
    if (crible == MAP_FAILED)
    {
        printf("Une erreur s'est produite!\n");
        return(1);
    }

    // Initialise le crible
    for(int i=0; i<n; i++)
    {
        crible[i] = 1;//par défaut: pas encore barré
    }

    // Créer n-2 processus enfants
    int pid = 0, num;
    for (int i = 2; i < n; i++)
    {
        pid = fork();

        // Les processus enfants sortent de la boucle
        if (pid == 0) {num = i; break;}
    }

    // Fin des processus enfants
    if (pid == 0) 
    {
        rayer_multiples(crible, n, num);
        exit(0);
    }

    // Attends que les processus enfants se terminent
    for (int i = 2; i < n; i++)
    {
        wait(NULL);
    }

    // Exercice 1 et 2 
    // for(int k=2; k<n; k++)
    // {
    //     rayer_multiples(crible, n, k);
    // }

    afficher(crible, n);
    
    return 0;
}