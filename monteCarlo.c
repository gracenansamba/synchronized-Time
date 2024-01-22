/*


 * Calculates digits of PI Using the Monte Carlo Method


 * 


 * 


*/


// Set-up: module load openmpi


// Compiler: mpicc monte_carlo_pi_mpi.c -o monte


// 


// Execute mpiexec -np 2 monte





#include "mpi.h"


#include <stdio.h>


#include <stdlib.h>


#include <math.h>


#include <time.h>


#include <sys/time.h>





long monteCarlo(long iter_max, int rank, int size)


{


    long n = 0, iter;


    double x, y, z;


    unsigned int seed = (unsigned)time(NULL) + rank;





    for (iter = 0; iter < iter_max; iter++)


    {


        x = ((double)rand_r(&seed) / RAND_MAX) * 2 - 1;


        y = ((double)rand_r(&seed) / RAND_MAX) * 2 - 1;


        z = sqrt((x * x) + (y * y));





        if (z <= 1)


            n++;


    }





    return n;


}





int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)


{


    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);


    result->tv_sec = diff / 1000000;


    result->tv_usec = diff % 1000000;


    return (diff < 0);


}





int main(int argc, char **argv)


{


    struct timeval tvBegin, tvEnd, tvDiff;





    MPI_Init(&argc, &argv);





    int rank, size;


    double pi;


    int ROWS, COLS;


    long reduced_count, local_count, N;





    MPI_Comm_size(MPI_COMM_WORLD, &size);


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);





    if (argc != 4)


    {


        if (rank == 0)


            printf("\nUsage: mpiexec -np NP monte dimRows dimCols niter\n");


        MPI_Finalize();


        exit(1);


    }


    else


    {


        sscanf(argv[1], "%d", &ROWS);


        sscanf(argv[2], "%d", &COLS);


        sscanf(argv[3], "%ld", &N);


    }





    long total_iter = (ROWS * COLS) * N * size;





    gettimeofday(&tvBegin, NULL);


    local_count = monteCarlo(N, rank, size);





    MPI_Reduce(&local_count, &reduced_count, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);





    if (rank == 0)


    {


        gettimeofday(&tvEnd, NULL);


        timeval_subtract(&tvDiff, &tvEnd, &tvBegin);


        printf("\nM=%d,T=%d,N=%ld: %ld.%06ld seconds elapsed\n", size, (ROWS * COLS), N, tvDiff.tv_sec, (long)tvDiff.tv_usec);


        pi = ((double)reduced_count / (double)(total_iter)) * 4;


        printf("The Value of Pi: %f\n", pi);


    }





    MPI_Finalize();





    return 0;


}



