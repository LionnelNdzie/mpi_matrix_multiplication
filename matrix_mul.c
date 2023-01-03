#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "functions.h"

int main(int argc, char** argv){
    int comm_sz;
    int my_rank;

    int rows_a, cols_a, rows_b, cols_b;
    
    double** matrix_a; // The first matrix
    double** matrix_b; // The second matrix

    double* b_array; // Store the conversion of matrix_b(2D) into an array(1D)
    
    double* global_a_array; // Store the conversion of matrix_a(2D) into an array(1D)
    double* local_a_array; // The part of global_a_array that each process will receive
    double** local_a_matrix; // Will corresponding slice of the matrix a that each process received

    double* global_result_array; // The array that will store the multiplication of the matrices
    double* local_result_array;  // The result of the local calculation of each process
    double** global_result_matrix; // Will store the glocal result matrix
    double** local_result_matrix; // Will store the local result matrix

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    if(my_rank == 0) {
        matrix_a = readMatrix(argv[1], &rows_a, &cols_a);

        if((rows_a % comm_sz) != 0) { // If the number of rows of matrix a is not evenly devided by the number of processes
            printf("Error!!! The number of rows should be a multiple of comm_sz.\n");
            MPI_Finalize();
            exit(0);
        }

        matrix_b = readMatrix(argv[2], &rows_b, &cols_b);

        if(cols_a != rows_b) { // If the two matrices cannot be multiplied
            printf("Error!!! Those matrices cannot be multiplied.\n");
            MPI_Finalize();
            exit(0);
        }

        /* We make this conversion as matrices cannot be sent in MPI */
        b_array = matrixToArray(matrix_b, rows_b, cols_b);
    }

    /* We create the derived type */
    int lengths[] = {1, 1, 1, 1};

    MPI_Aint rows_a_addr, cols_a_addr, rows_b_addr, cols_b_addr;
    MPI_Get_address(&rows_a, &rows_a_addr); MPI_Get_address(&cols_a, &cols_a_addr);
    MPI_Get_address(&rows_b, &rows_b_addr); MPI_Get_address(&cols_b, &cols_b_addr);
    MPI_Aint dsplmnts[] = {0, cols_a_addr - rows_a_addr, rows_b_addr - rows_a_addr, cols_b_addr - rows_a_addr};

    MPI_Datatype types[] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};

    MPI_Datatype rows_and_cols; // The new datatype

    MPI_Type_create_struct(4, lengths, dsplmnts, types, &rows_and_cols);
    MPI_Type_commit(&rows_and_cols);
    MPI_Bcast(&rows_a, 1, rows_and_cols, 0, comm);
    MPI_Type_free(&rows_and_cols);

    /* The other processes should initialize the arrays */
    if (my_rank != 0)
    {
        matrix_b = malloc(rows_b * sizeof(double*));
        for(int i = 0; i < rows_b; i++)
            matrix_b[i] = malloc(cols_b * sizeof(double));

        b_array = malloc((rows_b * cols_b) * sizeof(double));
    }

    double local_start, local_finish, local_elapsed, elapsed;
    local_start = local_finish = local_elapsed = elapsed = 0;

    MPI_Barrier(comm);
    local_start = MPI_Wtime();
    
    /* All the processes need the entire b matrix */
    MPI_Bcast(b_array, rows_b*cols_b, MPI_DOUBLE, 0, comm);
    if (my_rank == 0)
        free(b_array); // Because no longer needed on process 0

    /* All the other processes retrieve the matrix b from the array they received */
    if (my_rank != 0)
    {
        matrix_b = arrayToMatrix(b_array, rows_b, cols_b);
        free(b_array); // Because no longer needed
    }

    int local_a_array_sz = cols_a * (rows_a / comm_sz); // The number of elements of matrix A that each process will get

    local_a_array = malloc(local_a_array_sz * sizeof(double)); // If the pointer is NULL, we will have a segmentation error

    if(my_rank == 0) {
        global_a_array = matrixToArray(matrix_a, rows_a, cols_a);

        MPI_Scatter(global_a_array, local_a_array_sz, MPI_DOUBLE, local_a_array, local_a_array_sz, MPI_DOUBLE, 0, comm);
        free(global_a_array); // Because no longer needed on process 0
    }
    else {
        MPI_Scatter(global_a_array, local_a_array_sz, MPI_DOUBLE, local_a_array, local_a_array_sz, MPI_DOUBLE, 0, comm);
    }

    local_a_matrix = arrayToMatrix(local_a_array, rows_a/comm_sz, cols_a);
    free(local_a_array); // because no longer needed on all the processes

    /* ---------------------------------------------------------------------------------------------*/
    local_result_matrix = mulMatrix(local_a_matrix, rows_a/comm_sz, cols_a, matrix_b, rows_b, cols_b);

    for (int i = 0; i < rows_a/comm_sz; ++i)
        free(local_a_matrix[i]);

    for (int i = 0; i < rows_b; ++i)
        free(matrix_b[i]);

    local_result_array = matrixToArray(local_result_matrix, rows_a/comm_sz, cols_b);

    if(my_rank == 0) {
        int send_cnt = rows_a/comm_sz * cols_b;
        global_result_array = malloc(rows_a * cols_b * sizeof(double));
        MPI_Gather(local_result_array, send_cnt, MPI_DOUBLE, global_result_array, send_cnt, MPI_DOUBLE, 0, comm);
        free(local_result_array);
    }
    else {
        int send_cnt = rows_a/comm_sz * cols_b;
        MPI_Gather(local_result_array, send_cnt, MPI_DOUBLE, global_result_array, send_cnt, MPI_DOUBLE, 0, comm);
        free(local_result_array);
    }
    /* ---------------------------------------------------------------------------------------------*/

    local_finish = MPI_Wtime();
    local_elapsed = local_finish - local_start;
    MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, comm);

    if(my_rank == 0) {
        global_result_matrix = arrayToMatrix(global_result_array, rows_a, cols_b);
        free(global_result_array);

        printf("The result of the multiplication is :\n");
        //printMatrix(global_result_matrix, rows_a, cols_b);
        for (int i = 0; i < rows_a; ++i)
            free(global_result_matrix[i]);
    }

    if (my_rank == 0)
        printf("Elapsed time = %lf seconds\n", elapsed);

    MPI_Finalize();

    return 1;
}