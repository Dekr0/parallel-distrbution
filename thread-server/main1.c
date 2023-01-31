#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"


pthread_mutex_t mutex;
char ** resources;


int setup(char * addr, long arraySize, long ip);

void * handle(void * args);


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: <array size> <server ip> <server port>\n");

        exit(1);
    }

    long arraySize = strtol(argv[1], NULL, 10);
    long port = strtol(argv[3], NULL, 10);

    int server = setup(argv[2], arraySize, port);

    pthread_t * threads;
    threads = malloc(COM_NUM_REQUEST * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);

    listen(server, COM_NUM_REQUEST);
    while (1) {
        for(int i = 0; i < COM_NUM_REQUEST; i++) {
            int client = accept(server, NULL, NULL);

            pthread_create(&threads[i],
                           NULL,
                           handle,
                           (void *) (long) client);
        }
    }

    return 0;
}

int setup(char * addr, long arraySize, long ip) {
    /* Create server socket */
    struct sockaddr_in sockVar;
    int server = socket(AF_INET, SOCK_STREAM, 0);

    sockVar.sin_addr.s_addr = inet_addr(addr);
    sockVar.sin_port = ip;
    sockVar.sin_family = AF_INET;

    if (bind(server, (struct sockaddr*)&sockVar, sizeof(sockVar)) < 0) {
        printf("Create socket failed");
        exit(1);
    }
    /* */

    /* Allocate memory for shared resources */
    resources = (char **) malloc(arraySize * sizeof(char *));
    for (int i = 0; i < arraySize; i++) {
        resources[i] = (char *) malloc(COM_BUFF_SIZE * sizeof(char));
        sprintf(resources[i], "String %d: the initial value", i);
        printf("%s\n", resources[i]);
    }
    /* */

    return server;
}


void * handle(void *args) {
    int client = (int)(long) args;

    char *receive = (char *) malloc(COM_BUFF_SIZE * sizeof(char));
    char *send = (char *) malloc(COM_BUFF_SIZE * sizeof(char));

    ClientRequest *request = (ClientRequest *) malloc(sizeof(ClientRequest));

    memset(receive, 0, COM_BUFF_SIZE);
    memset(send, 0, COM_BUFF_SIZE);
    memset(request, 0, sizeof(ClientRequest));

    read(client, receive, COM_BUFF_SIZE);

    ParseMsg(receive, request);
    
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

    write(client, send, COM_BUFF_SIZE);

    free(receive);
    free(send);

    close(client);
    return NULL;
}