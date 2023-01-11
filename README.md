# mpi_matrix_multiplication
This c program implements parallel multiplication of two matrices in C base on the MPI standard.
It is composed of a principal file, matrix_mul.
## Runnning
#### Matrices generation
1. To run it, you must first indicates in the file test.c the number of rows and columns of each of the two matrices. It should be noted that when planning
the orders of the matrices, the number of rows in the first matrix(matrix A) must be evenly divisible by the number of processes that we will run our mpi
program with. For example if the first matrix has 8 rows, the program will be able to run with 1, 2, 4 and 8 processes. 
2. Second you need to recompile the file test.c and to compile it you need to enter in the command line gcc -o a.out test.c functions.c as it uses the file 
functions.c 
3. Then execute with ./a.out
#### MPI parallel program compiling and running
It is assumed that one of the MPI implementations is installed on your machine(which should be running on a linux distro). For our case, we used mpich and 
it is the one that we urge you to install in order to avoid compatibility ussues. 
1. Compile the program with mpicc -o matrix_mul.out matrix_mul.c functions.c
2. Run the program with for example mpiexec -n 4 ./matrix_mul.out file_a file_b where file_a and file_b are where your files are stored
