#ifndef MATRIX_MULTIPLICATION_MAIN_H
#define MATRIX_MULTIPLICATION_MAIN_H


#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lab1_IO.h"
#include "timer.h"


typedef struct {
    int x;
    int y;
} BlockCoordinates;


typedef struct {
    int lower;
    int upper;
} Bound;


/**
 * Necessary arguments for each thread to perform matrix multiplication.
 *
 * @param A matrix A
 * @param B matrix B
 * @param C matrix C
 * @param k thread id
 * @param n matrix size
 * @param p number of threads
 */
typedef struct {
    int** A;
    int** B;
    int** C;
    int k;
    int n;
    int p;
} Arguments;


/**
 *
 * @param k rank of the thread
 * @param p number of threads
 * @return
 */
BlockCoordinates get_block(int k, int p);


Bound get_bound(int coord, int n, int p);


double single_thread_multiplication(int **A, int **B, int **C, int n);


double multi_thread_multiplication(int **A, int **B, int **C, int n, int p);


double serial_multi_thread_implementation(int **A, int **B, int **C, int n, int p);


int validate_thread_number(int p, int n);


void calculate_cells(int** A, int** B, int** C, Bound i_bound, Bound j_bound, int k);


void* block_calculation(void * arg);


#endif //MATRIX_MULTIPLICATION_MAIN_H