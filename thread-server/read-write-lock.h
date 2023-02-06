#ifndef THREAD_SERVER_READ_WRITE_LOCK_H
#define THREAD_SERVER_READ_WRITE_LOCK_H


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct {
    int numPendingWriters; // pending_writers
    int numReaders; // readers
    int writer; // writer
    pthread_cond_t readersProceed;
    pthread_cond_t writerProceed;
    pthread_mutex_t readWriteLock;
} ReadWriteLock;


void initReadWriteLock(ReadWriteLock * lock) {
    lock -> numPendingWriters = 0;
    lock -> numReaders = 0;
    lock -> writer = 0;

    pthread_cond_init(&(lock -> readersProceed), NULL);
    pthread_cond_init(&(lock -> writerProceed), NULL);
    pthread_mutex_init(&(lock -> readWriteLock), NULL);
}


/*
 * If there is a write lock or pending writers, perform condition wait,
 * else increment count of readers and grant read lock
 */
void readLock(ReadWriteLock * lock) {
    pthread_mutex_lock(&(lock -> readWriteLock));

    while ((lock -> numPendingWriters > 0) || (lock -> writer > 0)) {
        pthread_cond_wait(&(lock -> readersProceed), &(lock -> readWriteLock));
    }

    lock -> numReaders ++;

    pthread_mutex_unlock(&(lock -> readWriteLock));
}


/*
 * if there are readers or writers, increment pending writers count and wait.
 * On being woken, decrement pending writers count and increment writer count
 */
void writeLock(ReadWriteLock * lock) {
    pthread_mutex_lock(&(lock -> readWriteLock));

    while ((lock -> writer > 0) || (lock -> numReaders > 0)) {
        lock -> numPendingWriters ++;

        pthread_cond_wait(&(lock -> writerProceed), &(lock -> readWriteLock));

        lock -> numPendingWriters --;
    }

    lock -> writer++;

    pthread_mutex_unlock(&(lock -> readWriteLock));
}


/*
 * If there is a write lock then unlock, else if there are read locks,
 * decrement count of read locks. If the count is 0 and there is a pending
 * writer, let it through, else if there are pending readers, let them all
 * go through
 */
void unlockReadWriteLock(ReadWriteLock * lock) {
    pthread_mutex_lock(&(lock -> readWriteLock));

    if (lock -> writer > 0) {
        lock -> writer = 0;
    } else if (lock -> numReaders > 0) {
        lock -> numReaders --;
    }

    pthread_mutex_unlock(&(lock -> readWriteLock));

    if ((lock -> numReaders == 0) && (lock -> numPendingWriters > 0)) {
        pthread_cond_signal(&(lock -> writerProceed));
    } else if (lock -> numReaders > 0) {
        pthread_cond_broadcast(&(lock -> readersProceed));
    }

    printf("Unlock\n");
}

#endif
