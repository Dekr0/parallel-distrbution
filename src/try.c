#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "Lab3IO.h"
#include "timer.h"

int main () {

    int i, j, k, size, swap_index;
    int * index;

    double start, end;
    double ** Au;
	double * X;
    double max;
    double temp;

    Lab3LoadInput(&Au, &size);

    X = CreateVec(size);

    index = malloc(size * sizeof(int));
#pragma omp parallel for default(none) shared(size, index) private(i)
    for (i = 0; i < size; ++i) {
        index[i] = i;
    }

    GET_TIME(start);
    if (size == 1) {
        X[0] = Au[0][1] / Au[0][0];
    } else {

#pragma omp parallel default(none) shared(size, max, swap_index, Au, index) private(k, i, j, temp)
        for (k = 0; k < size; ++k) {
            max = 0;
			swap_index = 0;

#pragma omp single
            {
                for (i = k; i < size; ++i) {
                    if (max < Au[index[i]][k] * Au[index[i]][k]) {
                        max = Au[index[i]][k] * Au[index[i]][k];
                        swap_index = i;
                    }
                }

                if (swap_index != k)/*swap*/{
                    i = index[swap_index];
                    index[swap_index] = index[k];
                    index[k] = i;
                }
            }

#pragma omp for
			for (i = k + 1; i < size; ++i) {
				temp = Au[index[i]][k] / Au[index[k]][k];
				for (j = k; j < size + 1; ++j) {
                    Au[index[i]][j] -= Au[index[k]][j] * temp;
                }
			}       
        }

        for (k = size - 1; k > 0; --k) {
#pragma omp parallel for default (none) shared(Au, index, k, size) private(temp)
			for (i = k - 1; i >= 0; --i) {
				temp = Au[index[i]][k] / Au[index[k]][k];
				Au[index[i]][k] -= temp * Au[index[k]][k];
				Au[index[i]][size] -= temp * Au[index[k]][size];
			} 
		}

#pragma omp parallel for default(none) shared(index,Au,X,size)
        for (k = 0; k < size; ++k)
			X[k] = Au[index[k]][size] / Au[index[k]][k];
       
    }
    GET_TIME(end);
    
    Lab3SaveOutput(X, size, end-start);

    DestroyMat(Au, size);
    DestroyVec(X);
}
