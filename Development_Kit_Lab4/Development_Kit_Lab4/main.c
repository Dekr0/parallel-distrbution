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
int main(int argc, char* argv[])
{
    pageRank();
    return 0;
}

int pageRank(){
    struct node *nodehead;
    int nodecount;
    double *r, *r_pre, *contribution, *my_r, *my_contribution;
    int i, j, head, tail, val;
    double damp_const;
    int iterationcount = 0;
    double start, end;
    int numberOfElements, sizeOfBlock;
    FILE *ip;
    int numberOfProcesses, currentProcessRank;
    int flag = 1;

    if ((ip = fopen("data_input_meta","r")) == NULL) {
        printf("Error opening the data_input_meta file.\n");
        return 254;
    }
    fscanf(ip, "%d\n", &nodecount);
    fclose(ip);

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&numberOfProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD,&currentProcessRank);

    if (node_init(&nodehead, 0, nodecount)) return 254;

    sizeOfBlock = (int) (nodecount/numberOfProcesses);
    if((sizeOfBlock * numberOfProcesses) != nodecount)
        sizeOfBlock+=1;
    numberOfElements = sizeOfBlock * numberOfProcesses;

    my_r = malloc(sizeOfBlock * sizeof(double));
    r = malloc(numberOfElements * sizeof(double));
    r_pre = malloc(numberOfElements * sizeof(double));
    for ( i = 0; i < nodecount; ++i)
        r[i] = 1.0 / nodecount;
    
    my_contribution = malloc(sizeOfBlock * sizeof(double));
    contribution = malloc(numberOfElements * sizeof(double));
    for ( i = 0; i < nodecount; ++i)
        contribution[i] = r[i] / nodehead[i].num_out_links * DAMPING_FACTOR;
    damp_const = (1.0 - DAMPING_FACTOR) / nodecount;

    GET_TIME(start);

    do{
        if(currentProcessRank == 0){
            ++iterationcount;
            vec_cp(r, r_pre, nodecount);
        }
        
        head = currentProcessRank * sizeOfBlock;
        tail =( currentProcessRank+1) * sizeOfBlock -1;

        for(i = head; i < nodecount && i<= tail; i++){
            val = i-head;
            my_r[val] = 0;
            for ( j = 0; j < nodehead[i].num_in_links; j++){
                my_r[val] += contribution[nodehead[i].inlinks[j]];
            }
            my_r[val] += damp_const;
        }

        for(i = head; i < nodecount && i<= tail; i++){
            val = i-head;
            my_contribution[val] = my_r[val] / nodehead[i].num_out_links * DAMPING_FACTOR;
        }

        MPI_Gather(my_r, sizeOfBlock, MPI_DOUBLE, r, sizeOfBlock, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Allgather(my_contribution, sizeOfBlock, MPI_DOUBLE, contribution, sizeOfBlock, MPI_DOUBLE, MPI_COMM_WORLD);

        if(currentProcessRank == 0){
            flag = rel_error(r, r_pre, nodecount) >= EPSILON;
        }

        MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);        

    }
    while(flag);

    GET_TIME(end);

    MPI_Finalize();

    if(currentProcessRank ==0){
        Lab4_saveoutput(r, nodecount, end-start);
        node_destroy(nodehead, nodecount);
        free(contribution);
        free(my_contribution);
        free(r);
        free(my_r);
        free(r_pre);
    }

    return 0;

}