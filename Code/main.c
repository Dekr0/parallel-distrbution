#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

#include "Lab3IO.h"
#include "timer.h"


int main() {
    int i, j, k, size, swap_index;

    double start, end, max, tmp;
    double * swap = NULL;
    double * X;
    double ** Au;

    Lab3LoadInput(&Au, &size);

    X = CreateVec(size);

    GET_TIME(start);
#pragma omp parallel num_threads(12) default(none) shared(size, max, swap_index, Au) private(k, i, swap, tmp, j)
    for (k = 0; k < size - 1; k++) {
        /* Pivoting */
#pragma omp single
        {
            max = 0;
            swap_index = 0;
            swap = NULL;

            for (i = k; i < size; i++) {
                if (max < fabs(Au[i][k])) {
                    max = fabs(Au[i][k]);
                    swap_index = i;
                }
            }

            if (swap_index != k) {
                swap = Au[k];
                Au[k] = Au[swap_index];
                Au[swap_index] = swap;
            }
        }

        /* Gaussian */
#pragma omp for
        for (i = k + 1; i < size; i++) {
            tmp = Au[i][k] / Au[k][k];
            for (j = k; j < size + 1; j++) {
                Au[i][j] -= Au[k][j] * tmp;
            }
        }
    }

    for (k = size - 1; k > 0; k--) {
#pragma omp parallel for num_threads(12) default(none) shared(k, size, Au) private(i, tmp)
        for (i = k - 1; i >= 0; i--) {
            tmp = Au[i][k] / Au[k][k];
            Au[i][k] -= tmp * Au[k][k];
            Au[i][size] -= tmp * Au[k][size];
        }
    }

#pragma omp parallel for num_threads(12) default(none) shared(size, X, Au) private(k)
    for (k = 0; k < size; k++) {
        X[k] = Au[k][size] / Au[k][k];
    }

    GET_TIME(end);

//    printf("%f\n", end - start);

    Lab3SaveOutput(X, size, end - start);

    DestroyMat(Au, size);
    DestroyVec(X);
}
