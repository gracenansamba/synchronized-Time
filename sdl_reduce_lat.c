#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpix_harmonize.h>

#define ARRAY_SIZE 1000000
#define NUM_ITERATIONS 1000  // Replace with the actual number of iterations

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
    double start_time, end_time, t_sum = 0.0;
    int num_valid = 0;
    for (int iter = 0; iter < NUM_ITERATIONS; ++iter) {
       /* synchronize processes */
        //MPI_Barrier(MPI_COMM_WORLD);
	MPIX_Harmonize(MPI_COMM_WORLD, &flag);
        start_time = MPI_Wtime();
        int sum = 0;
        // Compute the sum

        for (int i = 0; i < ARRAY_SIZE; ++i) {
            sum += data[i];
        }

       // Record the end time
       end_time = MPI_Wtime();
        /* check whether this experiment is valid */
        int flag;
        MPI_Allreduce(MPI_IN_PLACE, &flag, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
        if (flag) { /* the experiment is valid */
            num_valid++;
            t_sum += end_time - start_time;
        }
    }
    if (rank == 0) {
        printf("MPI_Allreduce: %d valid iterations, %f average latency\n", num_valid, t_sum / num_valid);
    }
    free(data);
    MPI_Finalize();
    return 0;

}


