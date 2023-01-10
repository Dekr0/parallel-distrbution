#ifndef MATRIX_MULTIPLICATION_UTIL_H
#define MATRIX_MULTIPLICATION_UTIL_H


#include <stdio.h>
#include <stdlib.h>

/*
 * Read the matrix from the file
 */
int** read_matrix(FILE* fptr, int n);


/*
 * Read the dimension of the square matrix
 */
int read_matrix_size(FILE * fptr);


#endif //MATRIX_MULTIPLICATION_UTIL_H
