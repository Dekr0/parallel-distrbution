#ifndef SERVER_H
#define SERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

#include "common.h"
#include "timer.h"


#define PORT 3000


char **shared_array;


#endif