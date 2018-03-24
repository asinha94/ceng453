#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**************Defines**************/
#define MIN(a,b) ((a)<(b)?(a):(b))
#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) (BLOCK_LOW((id)+1),p,n) - BLOCK_LOW((id),p,n))
#define BLOCK_OWNER(index,p,n) (((p)*((index)+1)-1)/(n))

int main(int argc, char ** argv) {
    int global_count;
    int i, count;
    int index, prime, first, step;
    int local_count;
    
    int id, p;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    double elapsed_time =- MPI_Wtime();

    if (argc != 2) {
        if (!id) printf("Command Line: %s <m>\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }

    int n = atoi(argv[1]);
    int els = (n-1) / 2;

    int smaller_size = els/p;
    int larger_size = smaller_size + 1;
    int num_larger_blocks = els % p;
    int size;
    if (id < num_larger_blocks) {
        size = larger_size;
    } else {
        size = smaller_size;
    }
    
    int low_value = 2*(id*smaller_size + MIN(id, num_larger_blocks)) + 3;
    int high_value = low_value + 2*(size - 1);
    int proc0_size;

    if (num_larger_blocks > 0) {
        proc0_size = larger_size;
    } else {
        proc0_size = smaller_size;
    }

    if ((1 + 2*proc0_size) < (int) sqrt((double) n)) {
        if (!id) printf ("Too many processes, given upper bound of sieve\n");
        if (!id) printf ("proc0_size is %d and m is %d\n", proc0_size, n);
        MPI_Finalize();
        exit (1);
    }

    char * marked = (char *) malloc (size);

    if (marked == NULL) {
        printf ("Cannot allocate enough memory\n");
        MPI_Finalize();
        exit (1);
    }

    for (i = 0; i < size; i++) marked[i] = 0;
    if (!id) index = 0;
    prime = 3;
    do {
        if (prime * prime > low_value) {
            first = (prime * prime - low_value)/2;
        } else {
            int r = low_value % prime;
            if (!r) first = 0;
            else if ((prime - r) & 1)
                first = (2*prime - r)/2;
            else first = (prime - r)/2;
        }

        for (i = first; i < size; i += prime) marked[i] = 1;
        if (!id) {
            while (marked[++index]);
            prime = 2*index + 3;
        }
        MPI_Bcast (&prime,  1, MPI_INT, 0, MPI_COMM_WORLD);
    } while (prime * prime <= n);

    count = 0;
    for (i = 0; i < size; i++) if (!marked[i]) count++;

    MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    global_count++;
    elapsed_time += MPI_Wtime();

    if (!id) {
        printf ("There are %d primes less than or equal to %d\n", global_count, n);
        printf ("SIEVE_ODD (%d) %10.6f\n", p, elapsed_time);
    }
    MPI_Finalize ();
    return 0;

}