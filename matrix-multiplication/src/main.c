#include "main.h"


int main() {
    // Pointer to matrix A and matrix B
    int **A, **B;

    int n = 0; // Dimension of all matrices

    // Read the matrix A and matrix B from the input file
    int status = Lab1_loadinput(&A, &B, &n);
    if (status != 0) {
        return 1;
    }

    int p = n * n; // Number of threads

    // Allocate memory for resulting matrix
    int **C = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        C[i] = malloc(n * sizeof(int));
    }

    /* --- Matrix Calculation using Threading --- */
    pthread_t threads[p];

    double start = 0, end = 0;
    GET_TIME(start);

    for (int i = 0; i < p; ++i) {
        Arguments *args = malloc(sizeof(Arguments));
        args->A = A;
        args->B = B;
        args->C = C;
        args->p = p;
        args->k = i;
        status = pthread_create(&threads[i], NULL, cell_multiplication, args);
        if (status != 0) {
            return 1;
        }
    }

    for (int i = 0; i < p; ++i) {
        status = pthread_join(threads[i], NULL);
        if (status != 0) {
            return 1;
        }
    }

    GET_TIME(end);

    printf("Time (Threading): %f\n", end - start);


    /* --- Matrix Calculation using Single Thread --- */
    double runtime = verify_result(A, B, C, n);
    printf("TIme (Single Thread):%f\n", runtime);

    for (int i = 0; i < n; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
}


BlockCoordinates block_assignment(int k, int p) {
    BlockCoordinates block;

    block.x = (int) floor(k / sqrt(p));
    block.y = k % (int) sqrt(p);

    return block;
}


void* cell_multiplication(void* arg) {
    Arguments *args = (Arguments*) arg;

    const int **A = (const int**) args->A;
    const int **B = (const int**) args->B;
    int **C = (int**) args->C;
    const int p = args->p;
    const int k = args->k;

    printf("Thread %d is working\n", k);

    BlockCoordinates block = block_assignment(k, p);

    C[block.x][block.y] = A[block.x][block.y] * B[block.x][block.y];

    return 0;
}

double verify_result(int **A, int **B, int **C, int n) {
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
