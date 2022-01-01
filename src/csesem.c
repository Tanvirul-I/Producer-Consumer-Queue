#include <pthread.h>
#include <stdlib.h>

#include "csesem.h"

struct CSE_Semaphore {
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int done;
    int delete;
};

CSE_Semaphore csesem_create(int count) {
    if(count < 0) {
        return NULL;
    }
    
    CSE_Semaphore sem = calloc(1, sizeof(struct CSE_Semaphore));

    pthread_mutex_t mutex;
    pthread_cond_t cond;
    
    if(pthread_mutex_init(&mutex, NULL) != 0) {
        return NULL;
    }

    if(pthread_cond_init(&cond, NULL) != 0) {
        return NULL;
    }

    sem -> count = count;
    sem -> mutex = mutex;
    sem -> cond = cond;
    if(count == 0) sem -> done = 0;
    else sem -> done = 1;
    sem -> delete = 0;
    
    return sem;
}

void csesem_post(CSE_Semaphore sem) {
    pthread_mutex_lock(&sem -> mutex);
    sem -> count++;
    if(sem -> count > 0) {
        sem -> done = 1;
        pthread_cond_signal(&sem -> cond);
    }
    pthread_mutex_unlock(&sem -> mutex);
}

void csesem_wait(CSE_Semaphore sem) {
    pthread_mutex_lock(&sem -> mutex);
    if(sem -> count > 0) {
        sem -> count--;
    } else if(sem -> count == 0) {
        while(!sem -> done) {
            pthread_cond_wait(&sem -> cond, &sem -> mutex);
        }
        sem -> count--;
    }
    if(sem -> count == 0 && !sem -> delete) sem -> done = 0;
    pthread_mutex_unlock(&sem -> mutex);
}

void csesem_destroy(CSE_Semaphore sem) {
    pthread_mutex_lock(&sem -> mutex);
    sem -> done = 1;
    sem -> delete = 1;
    pthread_cond_broadcast(&sem -> cond);
    pthread_mutex_unlock(&sem -> mutex);
    
    pthread_mutex_destroy(&sem -> mutex);
    pthread_cond_destroy(&sem -> cond);
    free(sem);
}
