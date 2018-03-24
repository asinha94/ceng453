
#include "mpi.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char ** argv) {
    /* MPI Initialization */
    int i;
    int id;
    int p;
    
    int prid = getpid();
    int prsleep = prid  % 4;
    int sleep_time = 3;

    int bcast_share =123321;
    int *gather_buffer;
    int alllocation;
    
    clock_t tt = -clock();
    sleep(prsleep);
    tt += clock();
    printf("Process-ID: %d      Sleep-time: %d    Elapsed-time: %d\n");
    fflush(stdout);

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);


    /*  
    *  Broadcast Test
    */
    MPI_Barrier(MPI_COMM_WORLD);
    if (id == 0) {
        printf("BROADCAST TEST.... Number of Workers = %d", p);
        fflush(stdout);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    // Let random node sleep
    if (id == 1) sleep(sleep_time);
    // Send bcast_share to everyone
    MPI_Bcast(&bcast_share, 1, MPI_INT, 0, MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();
    printf("Process_BROADCAST %d elapsed time %8.6f  \n",id,elapsed_time);
    fflush(stdout);

    /*
    *   ALL GATHER TEST
    */
    MPI_Barrier(MPI_COMM_WORLD);
    if (id == 0) {
        printf("ALL GATHER TEST.... Number of Workers = %d", p);
        fflush(stdout);
    }

    gather_buffer = (int *) malloc (p * sizeof(int));
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    if (id == 1) sleep(sleep_time);

    MPI_Allgather(gather_buffer, 1, MPI_INT, gather_buffer, 1, MPI_INT, MPI_COMM_WORLD);

}