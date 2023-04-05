// to compile " mpicc -g -Wall -o main main.c Lab4_IO.c -lm "
// to run "mpirun -np 4 ./main"

#define LAB4_EXTEND
#include <mpi.h>
#include "Lab4_IO.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

int pageRank();


int main(int argc, char* argv[]) {
    pageRank();
    return 0;
}


int pageRank() {
    struct node * nodeHead;

    int iterationCount = 0;
    int nodeCount;
    int numberOfElements, sizeOfBlock;
    int numberOfProcesses, currentProcessRank;
    int i, j, head, tail, val;
    int flag = 1;

    double * r, * r_pre, * contribution, * local_r, * local_contribution;
    double damp_const;
    double start, end;

    FILE * ip;

    if ((ip = fopen("data_input_meta","r")) == NULL) {
        printf("Error opening the data_input_meta file.\n");
        return 254;
    }

    fscanf(ip, "%d\n", &nodeCount);
    fclose(ip);

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&numberOfProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD,&currentProcessRank);

    if (node_init(&nodeHead, 0, nodeCount)) return 254;

    sizeOfBlock = (int) (nodeCount / numberOfProcesses);
    if ((sizeOfBlock * numberOfProcesses) != nodeCount) {
        sizeOfBlock += 1;
    }

    numberOfElements = sizeOfBlock * numberOfProcesses;

    local_r = malloc(sizeOfBlock * sizeof(double));
    r = malloc(numberOfElements * sizeof(double));
    r_pre = malloc(numberOfElements * sizeof(double));
    for (i = 0; i < nodeCount; ++i) {
        r[i] = 1.0 / nodeCount;
    }
    
    local_contribution = malloc(sizeOfBlock * sizeof(double));
    contribution = malloc(numberOfElements * sizeof(double));
    for (i = 0; i < nodeCount; ++i) {
        contribution[i] = r[i] / nodeHead[i].num_out_links * DAMPING_FACTOR;
    }

    damp_const = (1.0 - DAMPING_FACTOR) / nodeCount;

    GET_TIME(start);
    do {
        if (currentProcessRank == 0) {
            ++iterationCount;
            vec_cp(r, r_pre, nodeCount);
        }
        
        head = currentProcessRank * sizeOfBlock;
        tail = (currentProcessRank + 1) * sizeOfBlock - 1;

        for (i = head; i < nodeCount && i<= tail; i++) {
            val = i-head;
            local_r[val] = 0;
            for (j = 0; j < nodeHead[i].num_in_links; j++) {
                local_r[val] += contribution[nodeHead[i].inlinks[j]];
            }
            local_r[val] += damp_const;
        }

        for (i = head; i < nodeCount && i<= tail; i++) {
            val = i - head;
            local_contribution[val] = local_r[val] / nodeHead[i].num_out_links * DAMPING_FACTOR;
        }

        MPI_Gather(local_r, sizeOfBlock, MPI_DOUBLE, r, sizeOfBlock, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        MPI_Allgather(local_contribution, sizeOfBlock, MPI_DOUBLE, contribution, sizeOfBlock, MPI_DOUBLE, MPI_COMM_WORLD);

        if (currentProcessRank == 0) {
            flag = rel_error(r, r_pre, nodeCount) >= EPSILON;
        }

        MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);        

    } while(flag);
    
    GET_TIME(end);

    printf("%f", end - start);

    MPI_Finalize();

    if (currentProcessRank == 0) {
        Lab4_saveoutput(r, nodeCount, end-start);
        node_destroy(nodeHead, nodeCount);
        free(contribution);
        free(local_contribution);
        free(r);
        free(local_r);
        free(r_pre);
    }

    return 0;

}
