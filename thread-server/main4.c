#include "read-write-lock.h"
#include "server.h"


pthread_mutex_t mutex;
ReadWriteLock readWriteLock;

char ** resources;


int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: <array size> <server ip> <server port>\n");

        exit(1);
    }

    long size = strtol(argv[1], NULL, 10);
    long port = strtol(argv[3], NULL, 10);

    int serverFD = setup(argv[2], port);

    resources = malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++) {
        resources[i] = malloc(COM_BUFF_SIZE * sizeof(char));

        sprintf(resources[i], "String %d: the initial value", i);

        printf("%s\n", resources[i]);
    }

    pthread_t * threads;
    threads = malloc(COM_NUM_REQUEST * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);
    initReadWriteLock(&readWriteLock);

    while (1) {
        for (int i = 0; i < COM_NUM_REQUEST; i++) {
            int clientFD = accept(serverFD, NULL, NULL);

            pthread_create(
                    &threads[i],
                    NULL,
                    handle,
                    (void *)(long) clientFD
            );
        }
    }

    return 0;
}


void * handle(void * args) {
    int clientFD = (int)(long) args;

    char * received = malloc(COM_BUFF_SIZE * sizeof(char));
    char * response = malloc(COM_BUFF_SIZE * sizeof(char));

    memset(received, 0, COM_BUFF_SIZE);
    memset(response, 0, COM_BUFF_SIZE);

    ClientRequest * r = malloc(sizeof(ClientRequest));

    memset(r, 0, sizeof(ClientRequest));

    read(clientFD, received, COM_BUFF_SIZE);

    ParseMsg(received, r);

    // TODO: deadlock issue
    // TODO: Convert this to an array of ReadWriteLock

    /* Critical Section */
    if (r->is_read) {
        readLock(&readWriteLock);
        getContent(response, r->pos, resources);
        unlockReadWriteLock(&readWriteLock);
    } else {
        writeLock(&readWriteLock);
        setContent(r->msg, r->pos, resources);
        getContent(response, r->pos, resources);
        unlockReadWriteLock(&readWriteLock);
    }
    /* ---------------- */

    write(clientFD, response, COM_BUFF_SIZE);

    free(received);
    free(response);

    close(clientFD);

    return 0;
}
