#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpix_harmonize.h>

#define ARRAY_SIZE 10000
#define NUM_ITERATIONS 100

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
    double program_start_time, program_end_time, computation_start_time, computation_end_time, t_sum = 0.0;
    int num_valid = 0;
    // Record the start time of the program
    program_start_time = MPI_Wtime();
    for (int iter = 0; iter < NUM_ITERATIONS; ++iter) {
        /* synchronize processes */
	computation_start_time = MPI_Wtime();
        MPI_Barrier(MPI_COMM_WORLD);
	//MPIX_Harmonize(MPI_COMM_WORLD, &flag);
        // Record the start time for computation
       
        int sum = 0;
        // Compute the sum
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            sum += data[i];
        }
        // Record the end time for computation
        computation_end_time = MPI_Wtime();
        /* check whether this experiment is valid */
	//MPIX_Harmonize(MPI_COMM_WORLD, &flag);
        MPI_Allreduce(MPI_IN_PLACE, &flag, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
        if (flag) { /* the experiment is valid */
            num_valid++;
            t_sum += computation_end_time - computation_start_time;
        }
    }
    // Record the end time of the program
    program_end_time = MPI_Wtime();
    if (rank == 0) {
        printf("MPI_Allreduce: %d valid iterations, %f average latency\n", num_valid, t_sum / num_valid);
        printf("Computation time: %f seconds\n", t_sum);
        printf("Total time of the program: %f seconds\n", program_end_time - program_start_time);
    }
    free(data);
    MPI_Finalize();
    return 0;
}


