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


/**
 *
 * @param k rank of the thread
 * @param p number of threads
 * @return
 */


void* cell_multiplication(void * arg);


double verify_result();


#endif //MATRIX_MULTIPLICATION_MAIN_H