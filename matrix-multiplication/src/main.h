#ifndef MATRIX_MULTIPLICATION_MAIN_H
#define MATRIX_MULTIPLICATION_MAIN_H


#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "lab1_IO.h"
#include "timer.h"


typedef struct {
    int x;
    int y;
} BlockCoordinates;


/**
 * Necessary arguments for each thread to perform matrix multiplication.
 *
 * @param A matrix A
 * @param B matrix B
 * @param C matrix C
 * @param n matrix size
 * @param p number of threads
 */
typedef struct {
    int** A;
    int** B;
    int** C;
    int p;
    int k;
} Arguments;


/**
 *
 * @param k rank of the thread
 * @param p number of threads
 * @return
 */
BlockCoordinates block_assignment(int k, int p);


void* cell_multiplication(void * arg);


double single_thread_multiplication(int **A, int **B, int **C, int n);


#endif //MATRIX_MULTIPLICATION_MAIN_H