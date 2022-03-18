#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h> /* wait() */
#include <unistd.h> /* fork() */
// Affiche les nombres premiers plus petits que n
void afficher(char* crible, int n)
{
    printf("Nombres premiers:");

    // Inutile de tester 0 et 1 
    for (int i = 2; i < n; i++) 
    {
        // Si le nombre est premier
        if (crible[i] == 1) { printf(" %d", i); }
    }
    printf("\n");
}

// Raye (i.e. passe à zéro) tous les multiples de k qui sont plus petits que n
void rayer_multiples(char* crible, int n, int k)
{
    // TODO: Optimisation possible avec sqrt(n) ?
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
    assert(crible != MAP_FAILED);

    // Initialise le crible
    for(int i=0; i<n; i++)
    {
        crible[i] = 1;//par défaut: pas encore barré
    }

    // Créer n-2 processus enfants
    for (int i = 2; i < n; i++) {

        // Créer un nouveau processus + calcul des multiples de i
        if (fork() == 0) {
            rayer_multiples(crible, n, i);
            exit(0); // Évite l'explosion de processus
        }
    }

    // Attends que les processus enfants se terminent
    for (int i = 2; i < n; i++) { wait(NULL); }

    // Exercice 1 et 2 
    // for(int k=2; k<n; k++)
    // {
    //     rayer_multiples(crible, n, k);
    // }

    // Affiche les résultats
    afficher(crible, n);
    
    return 0;
}
