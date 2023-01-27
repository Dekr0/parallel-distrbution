#ifndef THREAD_SERVER_SERVER_H
#define THREAD_SERVER_SERVER_H


#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"


volatile sig_atomic_t terminate = 0;

char **resources;

pthread_t *threads;

void terminateHandler(int signum) {
    terminate = 1;
}


int setup(char *argv[3]) {
    signal(SIGINT, terminateHandler);

    int port = (int) strtol(argv[3], NULL, 10);
    int size = (int) strtol(argv[1], NULL, 10);

    struct sockaddr_in sockVar;
    int server = socket(AF_INET, SOCK_STREAM, 0);

    sockVar.sin_addr.s_addr = inet_addr(argv[2]);
    sockVar.sin_port = port;
    sockVar.sin_family = AF_INET;

    int bindResult = bind(
            server,
            (struct sockaddr *) &sockVar,
                    sizeof(sockVar));

    if (bindResult >= 0) {
        printf("socket has been created\n");
        listen(server, COM_NUM_REQUEST);

        threads = malloc(size * sizeof(pthread_t));

        // Initialize resources
        resources = malloc(size * sizeof(char *));
        for (int i = 0; i < size; i++) {
            resources[i] = malloc(COM_BUFF_SIZE * sizeof(char));

            sprintf(resources[i], "String %d: the initial value", i);

            printf("Resource %d: %s\n", i, resources[i]);
        }

        return server;
    } else {
        printf("socket creation failed\n");

        return -1;
    }
}


void cleanup(int server) {
    for (int i = 0; i < COM_NUM_REQUEST; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);

    for (int i = 0; i < COM_NUM_REQUEST; i++) {
        free(resources[i]);
    }

    free(resources);

    close(server);
}


#endif //THREAD_SERVER_SERVER_H
