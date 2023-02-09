#include "server.h"


pthread_mutex_t mutex;
char ** resources;
double * latencies;


int main(int argc, char *argv[]) {
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

    latencies = malloc(COM_NUM_REQUEST * sizeof(double));

    pthread_t * threads;
    threads = malloc(COM_NUM_REQUEST * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);

    while (1) {
        memset(latencies, 0, COM_NUM_REQUEST);

        for (int i = 0; i < COM_NUM_REQUEST; i++) {
            int client = accept(serverFD, NULL, NULL);
            if (client < 0) {
                printf("Error %d on accepting incoming client\n", client);
            } else {
                Data * data = malloc(sizeof(Data));

                data -> client = (long) client;
                data -> requestId = i;

                int code = pthread_create(
                        &threads[i],
                        NULL,
                        handle,
                        (void *) data
                );

                if (code != 0) {
                    printf("Error %d on creating a new thread for client %d\n", code, client);
                }
            }
        }

//        printf("Finish accept all %d clients. Restarting...\n", COM_NUM_REQUEST);
//        sleep(2);

        for (int i = 0; i < COM_NUM_REQUEST; i++) {
            int code = pthread_join(threads[i], NULL);
            if (code != 0) {
                printf("Error %d on closing thread %d\n", code, i);
            }
        }

        saveTimes(latencies, COM_NUM_REQUEST);
    }

    return 0;
}


void * handle(void *args) {
    Data * data = (Data *) args;

    int client = (int) data -> client;
    int requestId = data -> requestId;

    char *receive = malloc(COM_BUFF_SIZE * sizeof(char));
    char *send = malloc(COM_BUFF_SIZE * sizeof(char));

    ClientRequest *request = malloc(sizeof(ClientRequest));

    memset(receive, 0, COM_BUFF_SIZE);
    memset(send, 0, COM_BUFF_SIZE);
    memset(request, 0, sizeof(ClientRequest));

    read(client, receive, COM_BUFF_SIZE);

    ParseMsg(receive, request);

    double start = 0.0, end = 0.0;

    GET_TIME(start);
    if(request->is_read){
        pthread_mutex_lock(&mutex);
        getContent(send, request->pos, resources );
        pthread_mutex_unlock(&mutex);
    } else{
        pthread_mutex_lock(&mutex);
        setContent(request->msg, request->pos, resources);
        getContent(send, request->pos, resources );
        pthread_mutex_unlock(&mutex);
    }
    GET_TIME(end);

    latencies[requestId] = end - start;

    write(client, send, COM_BUFF_SIZE);

    free(receive);
    free(send);
    free(request);
    free(data);

    close(client);
    return NULL;
}
