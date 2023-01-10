#include "main.h"


// Pointer to matrix A and matrix B
int **A, **B, **C;

// Dimension of all matrices, number of threads
int n, p = 0;


int main() {

    // Read the size of matrix, the matrix A and matrix B from the input file
    int status = Lab1_loadinput(&A, &B, &n);
    if (status != 0) {
        return 1;
    }

    p = n * n;

    // Allocate memory for resulting matrix
    C = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        C[i] = malloc(n * sizeof(int));

        memset(C[i], 0, n * sizeof(int));
    }


    printf("Number of threads: %d", p);
    printf("Dim of matrix: %d", n);


    /* --- Matrix Calculation using Threading --- */
    pthread_t threads[p];

    /* --- Single Threading --- */
}


void* cell_multiplication(void* arg) {
    const int k = *(int*) arg;

    printf("Thread %d is working\n", k);

    const int x = (int) floor(k / sqrt(p));
    const int y = k % (int) sqrt(p);

    C[x][y] = A[x][y] * B[x][y];

    return 0;
}

double verify_result() {
    double start = 0, end = 0;

    GET_TIME(start);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            assert(C[i][j] == A[i][j] * B[i][j]);
        }
    }
    GET_TIME(end);

    return end - start;
}
