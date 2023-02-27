#include "Lab3IO.h"
#include "timer.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[]){

    int i, j, k, size;
	double** Au;
	double* X;
    double temp;
    double start, end;
	int* index;
	FILE* fp;

    Lab3LoadInput(&Au, &size);
    GET_TIME(start);
    X = CreateVec(size);
    index = malloc(size * sizeof(int));
    #pragma omp_set_schedule(static);
    #pragma omp parallel for
    for (i = 0; i < size; ++i)
        index[i] = i;

    if (size == 1)
		X[0] = Au[0][1] / Au[0][0];
	else{
        //confirm number of threads we can/should use
        // #pragma omp parallel for private(temp,i) default(none) shared(size,Au,index)
        for(k = 0; k < size; ++k){
            temp = 0;
			j=0;
			#pragma omp parallal for firstprivate(j) lastprivate(j) shared(size, Au, index, k) default(none) 
            for (i = k; i < size; ++i)
				if (temp < Au[index[i]][k] * Au[index[i]][k]){
					temp = Au[index[i]][k] * Au[index[i]][k];
					j = i;
				}
			if (j != k)/*swap*/{
				i = index[j];
				index[j] = index[k];
				index[k] = i;
			}
			/*calculating*/
            #pragma omp parallel for default(none) shared(Au, index, size, k) private(temp,j) // faster when i dont mention num_threads or reduction
			for (i = k + 1; i < size; ++i){
				temp = Au[index[i]][k] / Au[index[k]][k];
				for (j = k; j < size + 1; ++j)
					Au[index[i]][j] -= Au[index[k]][j] * temp;
			}       
        }
        for (k = size - 1; k > 0; --k){
            #pragma omp parallel for default (none) shared(Au,index,k,size) private(temp)
			for (i = k - 1; i >= 0; --i ){
				temp = Au[index[i]][k] / Au[index[k]][k];
				Au[index[i]][k] -= temp * Au[index[k]][k];
				Au[index[i]][size] -= temp * Au[index[k]][size];
			} 
		}
        #pragma omp parallel for shared(index,Au,X,size) default(none) schedule (static) // faster when i dont mention num_threads
        for (k=0; k< size; ++k)
			X[k] = Au[index[k]][size] / Au[index[k]][k];
       
    }
    GET_TIME(end);
    
    Lab3SaveOutput(X,size,end-start);
}
