#ifndef PCQ_H_
#define PCQ_H_

typedef struct PCQueue *PCQueue;

PCQueue pcq_create(int slots);

void pcq_insert(PCQueue pcq, void *data);

void *pcq_retrieve(PCQueue pcq);

void pcq_destroy(PCQueue pcq);

#endif 
