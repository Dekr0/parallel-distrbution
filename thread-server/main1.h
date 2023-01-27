#ifndef THREAD_SERVER_MAIN1_H
#define THREAD_SERVER_MAIN1_H



#include "server.h"
#include "timer.h"


void *handle(void *arg);


void get(char *send, int pos);


void set(char *setMsg, char *send, int pos);


#endif
