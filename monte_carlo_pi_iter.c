#include <stdio.h>


#include <stdlib.h>


#include <mpi.h>


#include <time.h>





int main(int argc, char** argv) {


    MPI_Init(&argc, &argv);





    int rank, size;


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    MPI_Comm_size(MPI_COMM_WORLD, &size);





    long long total_iterations = 100;


    long long total_points = 1000000;


    long long points_inside_circle = 0;





    // Seed the random number generator differently for each process


    srand(time(NULL) + rank);





    MPI_Barrier(MPI_COMM_WORLD);


    double start_time = MPI_Wtime();





    for (long long iter = 0; iter < total_iterations; iter++) {


        for (long long i = 0; i < total_points; i++) {


            double x = ((double)rand() / RAND_MAX) * 2 - 1;


            double y = ((double)rand() / RAND_MAX) * 2 - 1;





            // Check if the point is inside the unit circle


            if (x*x + y*y <= 1) {


                points_inside_circle++;


            }


        }


    }





    double end_time = MPI_Wtime();





    // Sum up the counts from all processes


    long long total_inside_circle;


    MPI_Reduce(&points_inside_circle, &total_inside_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);





    if (rank == 0) {


        // Calculate Pi


        double pi = 4.0 * total_inside_circle / (total_points * size * total_iterations);


        printf("Estimated value of Pi: %f\n", pi);





        // Measure computation time and overall program time


        double computation_time = end_time - start_time;


        double program_time;


        MPI_Reduce(&computation_time, &program_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);





        printf("Computation Time: %f seconds\n", program_time);


    }





    MPI_Finalize();





    return 0;


}



