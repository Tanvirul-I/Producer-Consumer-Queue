#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "csesem.h"
#include "pcq.h"

struct PCQueue {
    int slots;
    int freeSlots;
    int headLocation;
    int tailLocation;
    int destroy;
    void **queue;
    CSE_Semaphore insert;
    CSE_Semaphore retrieve;
};

PCQueue pcq_create(int slots) {
    PCQueue pcq;

    if(slots < 0) return NULL;
    
    CSE_Semaphore insert = csesem_create(1);
    CSE_Semaphore retrieve = csesem_create(0);

    if(insert == NULL || retrieve == NULL) return NULL;
    
    pcq = calloc(1, sizeof(*pcq));
    pcq -> queue = calloc(slots, sizeof(void *));
    pcq -> slots = slots;
    pcq -> freeSlots = slots;
    pcq -> insert = insert;
    pcq -> retrieve = retrieve;
    pcq -> headLocation = 0;
    pcq -> tailLocation = -1;
    pcq -> destroy = 0;
    
    return pcq;
}

void pcq_insert(PCQueue pcq, void *data) {
    csesem_wait(pcq -> insert);

    if(pcq -> tailLocation == pcq -> slots - 1) pcq -> tailLocation = -1;

    if(pcq -> destroy) return;

    pcq -> queue[pcq -> tailLocation + 1] = data;

    pcq -> tailLocation++;

    pcq -> freeSlots--;

    csesem_post(pcq -> retrieve);
}

void *pcq_retrieve(PCQueue pcq) {
    csesem_wait(pcq -> retrieve);

    if(pcq -> destroy) return NULL;

    void *returnValue = pcq -> queue[pcq -> headLocation];
    
    if(pcq -> headLocation == pcq -> slots - 1) pcq -> headLocation = 0;
    else pcq -> headLocation++;

    pcq -> freeSlots++;
    
    csesem_post(pcq -> insert);
    
    return returnValue;
}

void pcq_destroy(PCQueue pcq) {
    free(pcq->queue);
    pcq -> destroy = 1;

    csesem_destroy(pcq -> insert);
    
    csesem_destroy(pcq -> retrieve);
    
    free(pcq);
}
