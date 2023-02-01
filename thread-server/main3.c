#include "read-write-lock.h"
#include "server.h"

pthread_mutex_t mutex;
ReadWriteLock readWriteLock;

char ** resources;

int main(int argc, char * argv[]) {
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

    while (1) {
        for(int i = 0; i < COM_NUM_REQUEST; i++) {
            int client = accept(serverFD, NULL, NULL);

            pthread_create(&threads[i],
                           NULL,
                           handle,
                           (void *) (long) client);
        }
    }

    return 0;
}

void * handle(void *args) {
    int client = (int)(long) args;

    char *receive = malloc(COM_BUFF_SIZE * sizeof(char));
    char *send = malloc(COM_BUFF_SIZE * sizeof(char));

    ClientRequest *request = malloc(sizeof(ClientRequest));

    memset(receive, 0, COM_BUFF_SIZE);
    memset(send, 0, COM_BUFF_SIZE);
    memset(request, 0, sizeof(ClientRequest));

    read(client, receive, COM_BUFF_SIZE);

    printf("%s\n", receive);

    ParseMsg(receive, request);
    
    if(request->is_read){
        readLock(&readWriteLock);
        getContent(send, request->pos, resources );
        unlockReadWriteLock(&readWriteLock);
    } else{
        writeLock(&readWriteLock);
        setContent(request->msg, request->pos, resources);
        getContent(send, request->pos, resources );
        unlockReadWriteLock(&readWriteLock);
    }

    write(client, send, COM_BUFF_SIZE);

    free(receive);
    free(send);

    close(client);
    return NULL;
}
