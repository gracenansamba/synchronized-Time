#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <mpix_harmonize.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size, flag;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long total_iterations = 100;
    long long total_points = 100000;
    long long points_inside_circle = 0;
    double start_time, end_time, start_compute_time, end_compute_time;;
    // Seed the random number generator differently for each process
    srand(time(NULL) + rank);
    start_time = MPI_Wtime();  

    for (long long iter = 0; iter < total_iterations; iter++) {
	 
	start_compute_time = MPI_Wtime();
	points_inside_circle = 0;;

        for (long long i = 0; i < total_points; i++) {
            double x = ((double)rand() / RAND_MAX) * 2 - 1;
            double y = ((double)rand() / RAND_MAX) * 2 - 1;
            if (x*x + y*y <= 1) {
                points_inside_circle++;
            }
        }

        // Sum up the counts from all processes at the end of each iteration
        long long total_inside_circle;
	
	//MPI_Barrier(MPI_COMM_WORLD);
        MPIX_Harmonize(MPI_COMM_WORLD, &flag);

        MPI_Reduce(&points_inside_circle, &total_inside_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	//end_compute_time = MPI_Wtime();

        if (rank == 0) {
            // Calculate Pi for each iteration
            double pi = 4.0 * total_inside_circle / (total_points * size);
            //printf("Iteration %lld - Estimated value of Pi: %f\n", iter, pi);
        }
	 end_compute_time = MPI_Wtime();
    }
   end_time = MPI_Wtime();
    if (rank == 0) {
	double computation_time = end_compute_time -start_compute_time;
        double program_time = end_time - start_time;
        printf("Computation Time: %f seconds\n Overall Program Time: %f seconds\n", computation_time,program_time);
    }
    MPI_Finalize();
    return 0;
}



