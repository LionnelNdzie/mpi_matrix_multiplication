#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

void generateMatrix(const char* filename, int nb_rows, int nb_cols) {
    FILE* file_stream = NULL;
    file_stream = fopen(filename, "w");

    if(file_stream) {
        fprintf(file_stream, "%d %d\n", nb_rows, nb_cols);
        int i = 0, j = 0;

        for (i = 0; i < nb_rows; ++i) {
            j = 0;
            for (j = 0; j < nb_cols - 1; ++j) {
                fprintf(file_stream, "%d ", i + j);
            }
            fprintf(file_stream, "%d\n", i + j);
        }

        fclose(file_stream);
    }
    else {
        printf("Error!!! Cannot open file\n");
    }
}

double** readMatrix(const char* filename, int* nb_rows, int* nb_cols) {
    FILE* file_stream = NULL;
    file_stream = fopen(filename, "r");

    double** matrix = NULL;

    if(file_stream) {
        fscanf(file_stream, "%d %d\n", nb_rows, nb_cols);

        matrix = malloc(*nb_rows * sizeof(double*));
        for(int i = 0; i < *nb_rows; i++) {
            matrix[i] = malloc(*nb_cols * sizeof(double));
        }


        for(int i=0; i < *nb_rows; i++) {
            for (int j = 0; j < (*nb_cols) - 1; j++)
            {
                fscanf(file_stream, "%lf ", &matrix[i][j]);
            }
            fscanf(file_stream, "%lf\n", &matrix[i][(*nb_cols) - 1]);
        }

        fclose(file_stream);
    }
    else {
        printf("Error!!! Cannot open file\n");
    }

    return matrix;
}

double** createMatrix(int nb_rows, int nb_cols) {
    double** matrix;

    matrix = malloc(sizeof(double*) * nb_rows);
    for (int i = 0; i < nb_rows; i++){
        matrix[i] = malloc(sizeof(double) * nb_cols);
    }
    
    for (int row = 0; row < nb_rows; row++){
        for (int col = 0; col < nb_cols; col++){
            printf("Enter element matrix[%d][%d]:", row, col);
            scanf("%lf", &matrix[row][col]);
        }
        
    }

    return matrix;
}

double* matrixToArray(double** A, int nb_rows, int nb_cols) {
    double* array;
    array = malloc(sizeof(double) * nb_rows * nb_cols);

    for(int row = 0; row < nb_rows; row++)
        for(int col = 0; col < nb_cols; col++) 
            array[nb_cols * row + col] = A[row][col];

    return array;  
}

double** arrayToMatrix(double* array, int nb_rows, int nb_cols) {
    double** matrix;
    matrix = malloc(nb_rows * sizeof(double*));

    for(int i = 0; i < nb_rows; i++)
        matrix[i] = malloc(nb_cols * sizeof(double));

    for (int row = 0; row < nb_rows; row++)
        for (int col = 0; col < nb_cols; col++)
            matrix[row][col] = array[nb_cols * row + col];
        

    return matrix;
}

double** mulMatrix(double** matrix_a, int rows_a, int cols_a, double** matrix_b, int rows_b, int cols_b) {
    double** result = malloc(rows_a * sizeof(double*));

    for(int i = 0; i < rows_a; i++)
        result[i] = malloc(cols_b * sizeof(double));

    for(int row = 0; row < rows_a; row++)
        for(int col = 0; col < cols_b; col++) {
            result[row][col] = 0;
            for(int k = 0; k < cols_a; k++)
                result[row][col] += matrix_a[row][k] * matrix_b[k][col];
        }

    return result;
}

void printArray(double* array, int nb_cols) {
    printf("\n");
    for(int i = 0; i < nb_cols; i++)
        printf("%16.2lf|", array[i]);
    printf("\n");
}

void printMatrix(double** A, int nb_rows, int nb_cols) {
    printf("\n");
    for(int row=0; row < nb_rows; row++){
        for (int col = 0; col < nb_cols; col++){
            printf("%16.2lf|", A[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}