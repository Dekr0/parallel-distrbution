#ifndef MATRIX_MULTIPLICATION_MAIN_H
#define MATRIX_MULTIPLICATION_MAIN_H


#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
    int n;
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


/*
 * Read the matrix from the file
 */
int** read_matrix(FILE* fptr, int n);


/*
 * Read the dimension of the square matrix
 */
int read_matrix_size(FILE * fptr);


int split(char *str, char c, char ***arr);

#endif //MATRIX_MULTIPLICATION_MAIN_H