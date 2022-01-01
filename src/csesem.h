#ifndef CSESEM_H_
#define CSESEM_H_

typedef struct CSE_Semaphore *CSE_Semaphore;

CSE_Semaphore csesem_create(int count);

void csesem_post(CSE_Semaphore sem);

void csesem_wait(CSE_Semaphore sem);

void csesem_destroy(CSE_Semaphore sem);

#endif
