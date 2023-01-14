#include "main.h"


int main(int argc, char* argv[]) {
    if (argc != 3)
        return 1;

    // Dimension of the matrix;
    // number of thread is given by users input, a square number and factor of n^2
    int n = 0, p = (int) strtol(argv[1], NULL, 10);

    char *mode = argv[2];

    // Pointer to matrix A, matrix B, and matrix C
    int **A, **B, **C;

    // Read the matrix A and matrix B from the input file
    int status = Lab1_loadinput(&A, &B, &n);
    if (status != 0) {
        return 1;
    }

    if (validate_thread_number(p, n) != 0) {
        return 1;
    }

    // Allocate memory for resulting matrix
    C = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        C[i] = malloc(n * sizeof(int));

        memset(C[i], 0, n * sizeof(int));
    }

    double runtime = 0;
    if (strcmp(mode, "s") == 0) {
//        runtime = serial_multi_thread_implementation(A, B, C, n, p);
        runtime = single_thread_multiplication(A, B, C, n);
    } else if (strcmp(mode, "p") == 0) {
        runtime = multi_thread_multiplication(A, B, C, n, p);
    } else {
        return 1;
    }

    Lab1_saveoutput(C, &n, runtime);

    for (int i = 0; i < n; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
}


BlockCoordinates get_block(int k, int p) {
    BlockCoordinates block;

    block.x = (int) floor(k / sqrt(p));
    block.y = k % (int) sqrt(p);

    return block;
}


Bound get_bound(int coord, int n, int p) {
    Bound bound;

    bound.lower = (int) (coord * n / sqrt(p));
    bound.upper = ((int) ((coord + 1) * n / sqrt(p))) - 1;

    return bound;
}


int validate_thread_number(int p, int n) {
    if (p <= 0)
        return 1;

    int sqrt_p = (int) sqrt(p);

    if (sqrt_p * sqrt_p != p || (n * n) % p != 0)
        return 1;

    return 0;
}


double single_thread_multiplication(int **A, int **B, int **C, int n) {
    double start = 0, end = 0;

    GET_TIME(start)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int check = 0;
            for (int k = 0; k < n; ++k) {
                check += A[i][k] * B[k][j];
            }
            C[i][j] = check;
        }
    }
    GET_TIME(end)

    return end - start;
}


double multi_thread_multiplication(int **A, int **B, int **C, int n, int p) {
    pthread_t threads[p];

    int status = 0;

    double start = 0, end = 0;
    GET_TIME(start)

    for (int i = 0; i < p; ++i) {
        Arguments *args = malloc(sizeof(Arguments));
        args->A = A;
        args->B = B;
        args->C = C;
        args->k = i;
        args->n = n;
        args->p = p;
        status = pthread_create(&threads[i], NULL, block_calculation, args);
        if (status != 0) {
            exit(1);
        }
    }

    for (int i = 0; i < p; ++i) {
        status = pthread_join(threads[i], NULL);
        if (status != 0) {
            exit(1);
        }
    }

    GET_TIME(end)

    return end - start;
}


double serial_multi_thread_implementation(int **A, int **B, int **C, int n, int p) {
    double start = 0, end = 0;

    GET_TIME(start)
    for (int k = 0; k < p; k++) {
        BlockCoordinates block = get_block(k, p);
        Bound i_bound = get_bound(block.x, n, p);
        Bound j_bound = get_bound(block.y, n, p);

        calculate_cells(A, B, C, i_bound, j_bound, n);
    }


    GET_TIME(end)

    return end - start;
}


void* block_calculation(void* arg) {
    Arguments *args = (Arguments*) arg;

    int **A = (int**) args->A;
    int **B = (int**) args->B;
    int **C = (int**) args->C;

    const int k = args->k, n = args->n, p = args->p;

    BlockCoordinates block = get_block(k, p);

    Bound i_bound = get_bound(block.x, n, p);
    Bound j_bound = get_bound(block.y, n, p);

    calculate_cells(A, B, C, i_bound, j_bound, n);

    return 0;
}


void calculate_cells(int** A, int** B, int** C, Bound i_bound, Bound j_bound, int n) {
    for (int i = i_bound.lower; i <= i_bound.upper; ++i) {
        for (int j = j_bound.lower; j <= j_bound.upper; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


