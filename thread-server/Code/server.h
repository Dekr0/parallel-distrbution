#ifndef THREAD_SERVER_SERVER_H
#define THREAD_SERVER_SERVER_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"
#include "timer.h"


typedef struct {
    long client;
    int requestId;
} Data;


int setup(char * address, long ip) {
    struct sockaddr_in sock;

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    sock.sin_addr.s_addr = inet_addr(address);
    sock.sin_port = ip;
    sock.sin_family = AF_INET;

    if (bind(fd, (struct sockaddr*)&sock, sizeof(sock)) < 0) {
        printf("Socket creation failed\n");

        exit(1);
    }

    listen(fd, COM_NUM_REQUEST);
    printf("Server listening on port %ld\n", ip);

    return fd;
}


void * handle(void *);


#endif
