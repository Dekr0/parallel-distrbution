#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"

#define STR_LEN 1000


int setupServer(int arraySize, char *serverIP, int port, char ***sharedArray) {
    struct sockaddr_in sockVar;
    int serverFD = socket(AF_INET, SOCK_STREAM, 0);

    sockVar.sin_addr.s_addr = inet_addr(serverIP);
    sockVar.sin_port = port;
    sockVar.sin_family = AF_INET;

    int code = bind(serverFD, (struct sockaddr*)&sockVar, sizeof(sockVar));
    listen(serverFD, COM_NUM_REQUEST);

    if (code < 0) {
        printf("Socket creation failed\n");
        exit(1);
    }

    printf("Server socket has been created\n");

    *sharedArray = (char **) malloc(arraySize * sizeof(char *));
    for (int i = 0; i < arraySize; i++) {
        (*sharedArray)[i] = (char *) malloc(STR_LEN * sizeof(char));

        // Initialize the array
        sprintf((*sharedArray)[i], "String %d: the initial value", i);
    }

    return serverFD;
}


#endif