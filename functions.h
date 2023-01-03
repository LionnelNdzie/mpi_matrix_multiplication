#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/**
 * Generate a matrix a store it into the file reference by filename
 */
void generateMatrix(const char* filename, int nb_rows, int nb_cols);

/**
 * Reads a mtrix from a text file and returns it
 */
double** readMatrix(const char* filename, int* nb_rows, int* nb_cols);

/**
 * Help inputing an array form the standard input
 */
double** createMatrix(int nb_rows, int nb_cols);

/**
 * As arrays are stored in row major order in c, this function transforms 
 * a 2D array into its equivalent 1D array with the formula 2D[i][j] = 1D[cols * i + j]
 */
double* matrixToArray(double** A, int nb_rows, int nb_cols);

/**
 * The opposite of the above function
 */
double** arrayToMatrix(double* array, int nb_rows, int nb_cols);

/**
 * In vect : the row vector of the left
 * In nb_cells : the number of the cells in the vector
 * In matrix : the matrix of the right
 * In nb_rows : the number of rows of the matrix
 * In nb_cols : the number of columns of the matrix
 */
double** mulMatrix(double** matrix_a, int rows_a, int cols_a, double** matrix_b, int rows_b, int cols_b);

/**
 * Prints an array
 */
void printArray(double* array, int nb_cols);

/**
 * Prints a matrix
 */
void printMatrix(double** A, int nb_rows, int nb_cols);

#endif