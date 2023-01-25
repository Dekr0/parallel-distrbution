#include "main1.h"


char **sharedArray;

pthread_mutex_t mutex;


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: <array size> <server ip> <server port>\n");

        exit(1);
    }

    int serverFD = setupServer(
            (int) strtol(argv[1], NULL, 10),
            argv[2],
            (int) strtol(argv[3], NULL, 10),
            &sharedArray);

    int counter = 0;

    double start = 0, finish = 0, elapsed = 0;

    pthread_t *threads = (pthread_t *) malloc(COM_NUM_REQUEST * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);

    while (1) {
        for (int i = 0; i < COM_NUM_REQUEST; i++) {
            int clientFD = accept(serverFD, NULL, NULL);
            printf("Connected to client %d\n",clientFD);
            pthread_create(&threads[i], NULL, handle, (void *) &clientFD);

            counter++;
            printf("Counter: %d\n", counter);
        }
    }
}


void get(char *buffer, int pos) {
    pthread_mutex_lock(&mutex);

    getContent(buffer, pos, sharedArray);

    pthread_mutex_unlock(&mutex);
}


void set(char *msg, char *updatedMsg,  int pos) {
    pthread_mutex_lock(&mutex);

    setContent(msg, pos, sharedArray);

    strcpy(updatedMsg, sharedArray[pos]);

    pthread_mutex_unlock(&mutex);
}


// TODO: Need fixed
void *handle(void *arg) {
    int clientFD = *((int *) arg);
    char buffer[COM_BUFF_SIZE];

    ClientRequest request = {};

    read(clientFD, buffer, COM_BUFF_SIZE);

    if (strlen(buffer) > 0 && ParseMsg(buffer, &request) != 0) {
            printf("Failed to handle client %d", clientFD);
    } else {
        printf("Client %d, Is Read: %d, Position: %d\n", clientFD, request.is_read, request.pos);
        write(clientFD, "Response", strlen("Response") + 1);
    }

    return NULL;
}