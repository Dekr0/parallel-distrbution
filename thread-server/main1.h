#ifndef THREAD_SERVER_MAIN1_H
#define THREAD_SERVER_MAIN1_H



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
#include "timer.h"


void *handle(void *arg);


void get(char *send, int pos);


void set(char *setMsg, char *send, int pos);


#endif
