#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int n = size, answer[1];
    int fact = 1;
    for ( int i = 1; i <= rank + 1; i++ ) {
        fact *= i;
    }
    printf("[%d] Fact is %d\n", rank, fact);
    MPI_Reduce(&fact, answer, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if ( rank == 0 ) {
        printf("The sum of factorials is %d\n", answer[0]);
    }
    return 0;
}