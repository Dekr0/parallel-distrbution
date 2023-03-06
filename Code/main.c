#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

#include "Lab3IO.h"
#include "timer.h"


int main(int argc, char * argv[]) {
    int i;
    int j;
    int k;
    int swap;
    int swap_index;
    int size;
    int * index_map;

    double start,end;
    double tmp;
    double max;
    double * X;
    double ** Au;

    long threads;

    Lab3LoadInput(&Au, &size);

    X = CreateVec(size);

    index_map = malloc(size * sizeof(int));
#pragma omp parallel for default(none) shared(size, index_map) private(i)
    for (i = 0; i < size; i++) {
        index_map[i] = i;
    }

    threads = strtol(argv[1], NULL, 10);

    GET_TIME(start);
#pragma omp parallel num_threads(threads) default(none) shared(size, max, swap_index, index_map, Au) private(k, i, swap, tmp, j)
    for (k = 0; k < size - 1; k++) {
        /* Pivoting */
        max = 0;
        swap_index = 0;

#pragma omp single
        {
            for (i = k; i < size; i++) {
                if (max < fabs(Au[index_map[i]][k])) {
                    max = fabs(Au[index_map[i]][k]);
                    swap_index = i;
                }
            }

            if (swap_index != k) {
                swap = index_map[swap_index];
                index_map[swap_index] = index_map[k];
                index_map[k] = swap;
            }
        }

        /* Gaussian */
#pragma omp for
        for (i = k + 1; i < size; i++) {
            tmp = Au[index_map[i]][k] / Au[index_map[k]][k];
            for (j = k; j < size + 1; j++) {
                Au[index_map[i]][j] -= Au[index_map[k]][j] * tmp;
            }
        }

    }

    for (k = size - 1; k > 0; k--) {
#pragma omp parallel for num_threads(threads) default(none) shared(k, size, index_map, Au) private(i, tmp)
        for (i = k - 1; i >= 0; i--) {
            tmp = Au[index_map[i]][k] / Au[index_map[k]][k];
            Au[index_map[i]][k] -= tmp * Au[index_map[k]][k];
            Au[index_map[i]][size] -= tmp * Au[index_map[k]][size];
        }
    }

#pragma omp parallel for num_threads(threads) default(none) shared(size, X, index_map, Au) private(k)
    for (k = 0; k < size; k++) {
        X[k] = Au[index_map[k]][size] / Au[index_map[k]][k];
    }

    GET_TIME(end);

    Lab3SaveOutput(X, size, end - start);

    DestroyMat(Au, size);
    DestroyVec(X);
}