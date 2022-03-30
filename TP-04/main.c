#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "mem.h"

int main(int argc, char *argv[])
{
    // Initialise la mémoire et affiche son contenu
    mem_init();
    mem_show_heap();

    // Alloue un bloc de 42 octets
    char* p = mem_alloc( 42 ); 
    assert( p != NULL ); // On s'attend à ce que le bloc soit alloué
    printf("allocated 42 bytes at %p\n", p);
    mem_show_heap();

    // Exercice 1 -- Vérification du bon fonctionnement de mem_alloc
    printf(" === Exercice 1 === \n");

    mem_init(); // Réinitialise la mémoire
    for (int i = 0; i < 6; i++)
    {
        p = mem_alloc(42);
        assert(p != NULL); // L'allocation doit être réussie
        printf("allocated 42 bytes at %p\n", p);
        mem_show_heap();
    }

    // Limite observée : le derniers bloc a une capacité de 400 octets
    // mais seulement 42 octets sont utilisés

    // On s'attends à l'allocation échouer car il n'y a plus de blocs libres
    p = mem_alloc(42); 
    assert(p == NULL); 

    mem_init(); // Réinitialise la mémoire
    p = mem_alloc(200);
    assert(p != NULL); // L'allocation doit être réussie
    printf("allocated 200 bytes at %p\n", p);
    mem_show_heap();

    // Deuxième allocation de 200 octets
    p = mem_alloc(200);
    assert(p == NULL); // On s'attends à l'échec de l'allocation
    mem_show_heap();

    // Exercice 2 -- implémentation de mem_release
    printf(" === Exercice 2 === \n");

    mem_init(); // Réinitialise la mémoire
    for (int i = 0; i < 6; i++)
    {
        p = mem_alloc(42);
        assert(p != NULL); // L'allocation doit être réussie
        printf("allocated 42 bytes at %p\n", p);
        mem_show_heap();
    }

    mem_release(p); // Libère la zone allouée
    mem_show_heap();

    // On peux effectuer une nouvelle allocation
    p = mem_alloc(42);
    assert(p != NULL); // L'allocation doit être réussie
    printf("allocated 42 bytes at %p\n", p);
    mem_show_heap();

    mem_init(); // Réinitialise la mémoire
    p = mem_alloc(200);
    assert(p != NULL); // L'allocation doit être réussie
    printf("allocated 200 bytes at %p\n", p);
    mem_show_heap();

    mem_release(p); // Libère la zone allouée
    mem_show_heap();

    p = mem_alloc(300);
    assert(p != NULL); // L'allocation doit être réussie
    printf("allocated 300 bytes at %p\n", p);
    mem_show_heap();


    // Exercice 3 -- découpage des blocs trop grands
    printf(" === Exercice 3 === \n");


    // Il est maintenant possible d'effectuer 8 allocations de 42 octets consécutives
    mem_init(); // Réinitialise la mémoire
    for (int i = 0; i < 8; i++)
    {
        p = mem_alloc_trunc(42);
        assert(p != NULL); // L'allocation doit être réussie
        printf("allocated 42 bytes at %p\n", p);
        mem_show_heap();
    }

    // Cherche la limite d'allocations
    mem_init(); // Réinitialise la mémoire
    for (int i = 0; i < 12; i++)
    {
        p = mem_alloc_trunc(42);
        assert(p != NULL); // L'allocation doit être réussie
        printf("allocated 42 bytes at %p\n", p);
        mem_show_heap();
    }

    // Impossible de réaliser une treizième allocation de 42 octets
    p = mem_alloc_trunc(42);
    assert(p == NULL); // L'allocation doit échouer

    // Pour effectuer une allocation de 60 octets 
    // Calcul de la nouvelle capacité : (60+7)/8 * 8 = 64
    // Ajout du header à cette capacité : 64 + 8 = 72
    // Calcul de la marge : 72 + 8 = 80 -> Pas de découpage
    // (inutile d'allouer si on a pas de place pour autre chose que le header)
    // Stratégie actuelle : Pas de découpage car la mémoire restante est inutilisable
    // Limite : Perte inutiles de mémoire, reporter le découpage sur une grande zone
    // peux être une solution.

    // Le test qui échoué était celui de l'allocation successive de 200, puis 300.
    // La fonction a été changé par soucis de simplicité. (Demande de revenir sur la moitié 
    // des questions qui devait échouées et maintenant fonctionne.)
    // Le problème qui apparait est que plus on alloue la mémoire plus on la fragmente et 
    // il devient impossible d'allouer de grandes quantités de mémoire de manière continue.
    // Il s'agit du problème de **fragmentation** de la mémoire.
}
