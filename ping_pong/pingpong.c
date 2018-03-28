#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char ** argv) {
    
    int id;
    int p;
    int * pingpong;
    int size;

    double elapsed_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (argc != 1) {
        if (!id) printf ("Command line: %s <m>\n", argv[0]);
        MPI_Finalize();
        exit (1);
   }

    if (!id) {
        size = atoi(argv[1]);
        pingpong = (int *) malloc(size);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    elapsed_time = -MPI_Wtime();
    // Send message back and forth
    MPI_Bcast (pingpong,  1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (pingpong,  1, MPI_INT, 1, MPI_COMM_WORLD);

    elapsed_time += MPI_Wtime();

    if (!id) {
        printf("Ping Pong RTT for %d bytes: %f One Way: %f\n", size, elapsed_time, elapsed_time/2);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}