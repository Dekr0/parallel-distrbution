#include "main1.h"


pthread_mutex_t mutex;


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: <array size> <server ip> <server port>\n");

        exit(1);
    }

    int client;
    int i;

    int server = setup(argv);
    if (server < 0) {
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);

    while (!terminate) {
        for(i = 0; i < COM_NUM_REQUEST; i++) {
            client = accept(server, NULL, NULL);

            pthread_create(&threads[i],
                           NULL,
                           handle,
                           (void *) (long) client);
        }
    }

    cleanup(server);

    return 0;
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
    }
    else{
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