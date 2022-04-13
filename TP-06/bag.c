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
    sem_init(&(bag->sem_cons), 0, 0); // there's no item in the bag
    sem_init(&(bag->sem_prod), 0, size - 1); // bag is empty at start
    sem_init(&(bag->mutex), 0, 1); // mutex to protect the bag

    return bag;
}

void bb_add(bag_t * bag, void * element)
{
    assert(bag != NULL);                 // sanity check

    // while( bag->count >= bag->size ) { } // CAUTION: this synchronization is bogus

    sem_wait(&(bag->sem_prod)); // wait for an empty slot
    
    sem_wait(&(bag->mutex)); // lock the bag
    assert( bag-> is_closed == 0 ); // adding to a closed bag is an error
    assert( bag->count < bag->size ); // sanity check
    // sem_post(&(bag->mutex)); // unlock the bag

    // sem_wait(&(bag->mutex)); // lock the bag
    bag->elem[bag->count] = element;
    bag->count += 1;
    sem_post(&(bag->mutex)); // unlock the bag

    sem_post(&(bag->sem_cons)); // signal that there's an item in the bag
}

void * bb_take(bag_t *bag)
{
    assert(bag != NULL); // sanity check

    // while( bag->count <= 0 ) { } // CAUTION: this synchronization is bogus
    sem_wait(&(bag->sem_cons));  // wait for an item

    sem_wait(&(bag->mutex)); // lock the bag
    assert( bag->count > 0); // sanity check
    // sem_post(&(bag->mutex)); // unlock the bag

    // sem_wait(&(bag->mutex)); // lock the bag
    bag->count -= 1;
    void *r = bag->elem[bag->count];
    bag->elem[bag->count] = NULL;
    sem_post(&(bag->mutex)); // unlock the bag

    sem_post(&(bag->sem_prod)); // signal that space is available
    
    usleep(10);// artificial delay to increase the occurence of race conditions
    return r;
}

void bb_close(bag_t *bag, int N)
{
    assert("not implemented" == 0);
}
