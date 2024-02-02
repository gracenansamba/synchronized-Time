#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <mpix_harmonize.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size, flag;
    int VECTOR_SIZE;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Create a random vector on each process
    //int local_vector[VECTOR_SIZE];
    if(argc !=2) {
       	if(rank==0) printf("\nUsage: mpirun -np NP vectorSize \n");
	MPI_Finalize();
	exit(1);
    }
    else {
       	sscanf(argv[1],"%d",&VECTOR_SIZE);	
    }
    int local_vector[VECTOR_SIZE];
    srand(rank);
    for (int i = 0; i < VECTOR_SIZE; ++i) {
        local_vector[i] = rand() % 10;
    }
    double start_sync_time = MPI_Wtime();
    MPIX_Harmonize(MPI_COMM_WORLD, &flag);
    //MPI_Barrier(MPI_COMM_WORLD);
    double end_sync_time = MPI_Wtime();

    // Sum the vectors using MPI_Allreduce
    int global_vector[VECTOR_SIZE];
    double start_time = MPI_Wtime();
    MPI_Allreduce(local_vector, global_vector, VECTOR_SIZE, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    double start_sync_time2 = MPI_Wtime();
    //MPI_Barrier(MPI_COMM_WORLD);
    MPIX_Harmonize(MPI_COMM_WORLD, &flag);
    double end_sync_time2 = MPI_Wtime();
    //double end_time = MPI_Wtime();
    // Print the result and timing on each process
   /* printf("Process %d - Local Vector: ", rank);
    for (int i = 0; i < VECTOR_SIZE; ++i) {
        printf("%d ", local_vector[i]);
    }
    printf("\n");
    printf("Process %d - Global Vector (Sum): ", rank);
   for (int i = 0; i < VECTOR_SIZE; ++i) {
        printf("%d ", global_vector[i]);
    }
    printf("\n");*/
    if (rank == 0) {    
	double sync_time =  end_sync_time -start_sync_time;
	double sync_time2 =  end_sync_time2 -start_sync_time2;
	double total_sync_time = sync_time + sync_time2;
	double program_time = end_time - start_time;
       
        printf("Synchronization Time: %f seconds\n", total_sync_time);
        printf("AllReduce Time: %f seconds\n", program_time);
    }
    MPI_Finalize();
    return 0;
}

