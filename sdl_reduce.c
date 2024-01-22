#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


#include <mpix_harmonize.h>
#define CHECK_EVERY 16

#define ARRAY_SIZE 10000000

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size, flag;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *data = (int *)malloc(ARRAY_SIZE * sizeof(int));

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        data[i] = i + rank * ARRAY_SIZE;
    }
    // Timing variables
    double start_time, end_time;
    // Synchronize before the computation
     MPI_Barrier(MPI_COMM_WORLD);

    // Record the start time
    start_time = MPI_Wtime();
    int sum = 0;
    // Compute the sum
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        sum += data[i];
    }

   // Record the end time
    end_time = MPI_Wtime();
   // Synchronize after the computation
   // MPI_Barrier(MPI_COMM_WORLD);
   
    MPIX_Harmonize(MPI_COMM_WORLD, &flag);
    double elapsed_time = end_time - start_time;
    int globalSum = 0;
    // Use MPI_Reduce to get the global sum
    MPI_Reduce(&sum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Global sum: %d\n", globalSum);
        printf("Elapsed time: %.2f seconds\n", elapsed_time);
    }

    free(data);
    MPI_Finalize();
    return 0;
}
