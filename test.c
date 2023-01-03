#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main(int argc, char** argv){
    int rows_a, cols_a, rows_b, cols_b;

    generateMatrix("input_a.txt", 4096, 4096);
    generateMatrix("input_b.txt", 4096, 4096);

    double** matrix_a = readMatrix("input_a.txt", &rows_a, &cols_a);
    double** matrix_b = readMatrix("input_b.txt", &rows_b, &cols_b);

    //printMatrix(matrix_a, rows_a, cols_a);
    //printMatrix(matrix_b, rows_b, cols_b);

    double** result = mulMatrix(matrix_a, rows_a, cols_a, matrix_b, rows_b, cols_b);
    
    //printMatrix(result, rows_a, cols_b);
    return 0;
}