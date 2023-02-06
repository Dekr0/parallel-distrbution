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

    initReadWriteLock(&readWriteLock);

    while (1) {
        for (int i = 0; i < COM_NUM_REQUEST; i++) {
            int client = accept(serverFD, NULL, NULL);
            if (client < 0) {
                printf("Error %d on accepting incoming client\n", client);
            } else {
                int code = pthread_create(&threads[i],
                                          NULL,
                                          handle,
                                          (void *) (long) client);
                if (code != 0) {
                    printf("Error %d on creating a new thread for client %d\n", code, client);
                }
            }
        }

        printf("Finish accept all %d clients. Restarting...\n", COM_NUM_REQUEST);
//        sleep(2);

        for (int i = 0; i < COM_NUM_REQUEST; i++) {
            int code = pthread_join(threads[i], NULL);
            if (code != 0) {
                printf("Error %d on closing thread %d\n", code, i);
            }
        }
    }

    return 0;
}


void * handle(void * args) {
    int client = (int)(long) args;

    char * received = malloc(COM_BUFF_SIZE * sizeof(char));
    char * send = malloc(COM_BUFF_SIZE * sizeof(char));

    ClientRequest * request = malloc(sizeof(ClientRequest));

    memset(received, 0, COM_BUFF_SIZE);
    memset(send, 0, COM_BUFF_SIZE);
    memset(request, 0, sizeof(ClientRequest));

    read(client, received, COM_BUFF_SIZE);

    ParseMsg(received, request);

    // TODO: deadlock issue
    // TODO: Convert this to an array of ReadWriteLock


    /* Critical Section */
    if (request->is_read) {
        readLock(&readWriteLock);
        printf("Client %d acquired read\n", client);

        getContent(send, request->pos, resources);

        printf("Client %d tries to unlock\n", client);
        unlockReadWriteLock(&readWriteLock);
    } else {
        writeLock(&readWriteLock);
        printf("Client %d acquired write\n", client);

        setContent(request->msg, request->pos, resources);
        getContent(send, request->pos, resources);

        printf("Client %d tries to unlock\n", client);
        unlockReadWriteLock(&readWriteLock);
    }
    /* ---------------- */

    write(client, send, COM_BUFF_SIZE);

    free(received);
    free(send);

    close(client);
    return NULL;
}