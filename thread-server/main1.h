#ifndef THREAD_SERVER_MAIN1_H
#define THREAD_SERVER_MAIN1_H


#include "timer.h"
#include "server.h"


void *handle(void *arg);


void get(char *buffer, int pos);


void set(char *msg, char *updatedMsg, int pos);


#endif
