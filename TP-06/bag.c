#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#include "bag.h"

bag_t * bb_create(int size)
{
    assert(size > 0);
    
    bag_t *bag=malloc(sizeof(bag_t));
    assert(bag != NULL);

    bag->elem = malloc( size * sizeof(void*));
    assert(bag->elem != NULL);

    bag->size  = size;
    bag->count = 0;         // 'count' == index of first free slot in 'elem'
    bag->is_closed = 0;

    // initialize semaphores
    sem_init(&(bag->full_slot), 0, 0); // there's no item in the bag
    sem_init(&(bag->empty_slot), 0, size); // bag is empty at start
    sem_init(&(bag->mutex), 0, 1); // mutex to protect the bag

    return bag;
}

void bb_add(bag_t * bag, void * element)
{
    assert(bag != NULL); // Sanity check
        
    assert( bag-> is_closed == 0 ); // Adding to a closed bag is an error

    sem_wait(&(bag->empty_slot)); // Wait for an empty slot
    
    sem_wait(&(bag->mutex));
    assert( bag->count < bag->size ); // Sanity check
    bag->elem[bag->count] = element;
    bag->count += 1;
    sem_post(&(bag->mutex));

    sem_post(&(bag->full_slot)); // signal that there's an item in the bag
}

void * bb_take(bag_t *bag)
{
    assert(bag != NULL); // Sanity check

    sem_wait(&(bag->mutex));
    if (bag->is_closed == 1 && bag->count == 0)
    {
        sem_post(&(bag->mutex));
        return NULL;
    }
    sem_post(&(bag->mutex));

    sem_wait(&(bag->full_slot)); // Wait for an item in the bag

    sem_wait(&(bag->mutex));
    // Process has ended, but I've been sleeping
    if (bag->count == 0)
    {
        sem_post(&(bag->mutex));
        return NULL;
    }

    assert( bag->count > 0); // Sanity check
    bag->count -= 1;
    void *r = bag->elem[bag->count];
    bag->elem[bag->count] = NULL;
    sem_post(&(bag->mutex));

    sem_post(&(bag->empty_slot)); // Signal that there's an empty slot
    
    usleep(10);// artificial delay to increase the occurence of race conditions
    return r;
}

void bb_close(bag_t *bag, int N)
{
    assert(bag->is_closed == 0);
    bag->is_closed = 1;

    sem_wait(&(bag->mutex));
    for (int i = 0; i < N; i++)
    {
        // Libère les éléments restants dans le sac
        sem_post(&(bag->full_slot));
    }
    sem_post(&(bag->mutex));
}
