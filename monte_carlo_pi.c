#include <stdio.h>


#include <stdlib.h>


#include <mpi.h>


#include <time.h>





int main(int argc, char** argv) {


    MPI_Init(&argc, &argv);





    int rank, size;


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    MPI_Comm_size(MPI_COMM_WORLD, &size);





    long long total_points = 1000000;


    long long points_inside_circle = 0;





    // Seed the random number generator differently for each process


    srand(time(NULL) + rank);





    for (long long i = 0; i < total_points; i++) {


        double x = ((double)rand() / RAND_MAX) * 2 - 1;


        double y = ((double)rand() / RAND_MAX) * 2 - 1;





        // Check if the point is inside the unit circle


        if (x*x + y*y <= 1) {


            points_inside_circle++;


        }


    }





    // Sum up the counts from all processes


    long long total_inside_circle;


    MPI_Reduce(&points_inside_circle, &total_inside_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);





    if (rank == 0) {


        // Calculate Pi


        double pi = 4.0 * total_inside_circle / (total_points * size);


        printf("Estimated value of Pi: %f\n", pi);


    }





    MPI_Finalize();





    return 0;


}



