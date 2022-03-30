#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>

#include "mem.h"

void * heap_base = NULL;// first address of the heap
void * heap_end  = NULL;// first address beyond the heap

int mem_initialized = 0;

// initialize the memory manager
void mem_init(void)
{
    // request memory from the kernel
    heap_base = mmap(NULL, 800, PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    assert(heap_base != MAP_FAILED);

    heap_end = heap_base + 800 ;

    // create some free blocks: five of 80 bytes and one of 400 bytes
    void *block_ptr;
    for(int i=0; i<5;i++)
    {
        block_ptr = heap_base + 80*i;
        *( (int64_t*)block_ptr ) = 80;
    }
    block_ptr = heap_base + 400;
    *( (int64_t*)block_ptr ) = 400;
    
    mem_initialized = 1;
}
// Version pour les exercices 1 et 2
void * mem_alloc(int64_t length)
{
    assert(mem_initialized == 1);
    
    // Ajuste la taille demandée 
    length = (length+7)/8*8 ; // Ajuste la taille demandée pour qu'elle soit multiple de 8
    length += 8; // Ajoute l'espace pour le header dans la mémoire allouée

    void *  block_ptr ;
    int64_t header ;
    int64_t size;   
    char    flags;
    
    // On commence en placant le pointeur à la base du tas
    block_ptr = heap_base;

    // Tant que le pointeur n'est pas arrivé à la fin du tas
    while(block_ptr < heap_end)
    {
        header = *( (int64_t*)block_ptr ); // Récupère les 8 premiers octets
        flags  = header & 0b111; // Ne conserve que les 3 derniers bits
        size = header & ~0b111; // Ignore les 3 derniers bits

        // Si le bloc est assez grand et libre => on alloue !
        if((flags == 0) && (size >= length)) {break;}

        // On passe au bloc suivant
        block_ptr += size;
    }

    // Aucun bloc valide a été trouvé => retourne NULL
    if(block_ptr >= heap_end) {return NULL;}

    flags = 0b001; // Marque le bloc comme utilisé
    header = size | flags;
    *((int64_t*)block_ptr) = header; // Écrit le nouveau header

    // Retourne l'adresse de la zone utilisable (sans le header)
    return block_ptr + 8; 
}

void * mem_alloc_trunc(int64_t length)
{
    assert(mem_initialized == 1);
    
    // Ajuste la taille demandée 
    length = (length+7)/8*8 ; // Ajuste la taille demandée pour qu'elle soit multiple de 8
    length += 8; // Ajoute l'espace pour le header dans la mémoire allouée

    void *  block_ptr ;
    int64_t header ;
    int64_t size;   
    char    flags;
    
    // On commence en placant le pointeur à la base du tas
    block_ptr = heap_base;

    // Tant que le pointeur n'est pas arrivé à la fin du tas
    while(block_ptr < heap_end)
    {
        header = *( (int64_t*)block_ptr ); // Récupère les 8 premiers octets
        flags  = header & 0b111; // Ne conserve que les 3 derniers bits
        size = header & ~0b111; // Ignore les 3 derniers bits

        // Si le bloc est assez grand et libre => on alloue !
        if((flags == 0) && (size >= length)) {break;}

        // On passe au bloc suivant
        block_ptr += size;
    }

    // Aucun bloc valide a été trouvé => retourne NULL
    if(block_ptr >= heap_end) {return NULL;}

    // Si le bloc est assez grand on le divise en deux
    if (size > length + 8)
    {
        // On crée un nouveau bloc de la taille demandée
        void * new_block_ptr = block_ptr + length; // Créer le pointeur du deuxième bloc
        *((int64_t*)new_block_ptr) = size - length; // Met à jour sa taille

        // On met à jour la taille du premier bloc
        size = length;
    }

    flags = 0b001; // Marque le bloc comme utilisé
    header = size | flags;
    *((int64_t*)block_ptr) = header; // Écrit le nouveau header

    // Retourne l'adresse de la zone utilisable (sans le header)
    return block_ptr + 8; 
}

void mem_release(void *ptr)
{
    // Vérifie que la mémoire a bien été initialisée
    assert( mem_initialized == 1);
    assert( ((int64_t)ptr % 8) == 0 );

    // Attention ptr pointe sur la zone utile, pas sur le header
    int64_t header = *((int64_t*)(ptr - 8)); 

    int64_t flags  = 0b000; // Marque le bloc comme libre
    int64_t size = header & ~0b111; // Récupère la taille du bloc

    // Écrit le nouveau header
    header = size | flags; 
    *((int64_t*)(ptr - 8)) = header;
}

void mem_show_heap(void)
{
    assert( mem_initialized == 1);
    
    void * block_ptr = heap_base;

    printf("heap_base = %p\n",heap_base);
    while(block_ptr < heap_end)
    {
        int64_t header = *( (int64_t*)block_ptr );
        char    flags  = header & 0b111;  //   keep only three least significant bits
        int64_t size   = header & ~0b111; // discard the three least significant bits
        if( (size < 8) ||
            (size%8 != 0) )
        {
            printf("error: block at %p has incorrect size %lld\n",block_ptr,size);
            exit(1);
        }

        printf("  block at %p: header=0x%08llx size=%lld flags=%d (%s)\n",
               block_ptr,header,size,flags,
               (flags ? "taken" : "free")
               );

        block_ptr += size; // move on to next block
    }

    printf("heap_end = %p\n",heap_end);

    //sanity check: a full heap traversal should reach *exactly* the end
    assert( block_ptr == heap_end); 
}
