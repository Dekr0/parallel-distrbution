#include "main.h"


int main(int argc, char* argv[]) {

    if (argc != 2) {
        return 1;
    }

    FILE* ptr = fopen(argv[1], "r");

    if (ptr == NULL) {
        return 1;
    }

    // size of the matrix
    int n = read_matrix_size(ptr);

    printf("Matrix size: %d\n", n);

    int** matrix = read_matrix(ptr, n);

    free(matrix);

    return 0;
}


BlockCoordinates block_assignment(int k, int p) {
    BlockCoordinates block;

    block.x = (int) floor(k / sqrt(p));
    block.y = k % (int) sqrt(p);

    return block;
}


int** read_matrix(FILE* fptr, int n) {
    int** matrix = (int**) malloc(sizeof(int*) * n);

    for (int i = 0; i < n; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * n);
    }

    const unsigned long size = sizeof(char) * n * 3;
    char* line = malloc(size);
    char* end;
    memset(line, 0, size);

    char c;
    for (int i = 0; i < n; i++) {
        fgets(line, (int) size, fptr);

        if (line[0] == '\n')
            continue;

        for (int j = 0; j < n; j++) {
            int value = (int) strtol(line, &end, 10);

            printf("%d ", value);
        }
    }

    return matrix;
}


int read_matrix_size(FILE * fptr) {
    int size;

    char line[64];

    fgets(line, 64, fptr);

    size = (int) strtol(line, NULL, 10);

    return size;
}


void* cell_multiplication(void* arg) {
    const int rank = *((int *) arg);

    return 0;
}
