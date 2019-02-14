#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, n;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if ( rank == 0 ) {
        
        printf("[Rank %d] Enter a number >> ", rank);
        scanf("%d", &n);
        MPI_Send(&n, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);     
        MPI_Recv(&n, 1, MPI_INT, size - 1, size - 1, MPI_COMM_WORLD, &status);
        n += 1;
        printf("[Rank %d] Got my boi back, I make him >> %d\n", rank, n);

    } else {
        
        MPI_Recv(&n, 1, MPI_INT, rank - 1, rank - 1, MPI_COMM_WORLD, &status);
        n += 1;
        printf("[Rank %d] I made n -> %d\n", rank, n);
        if( rank + 1 != size) {
            MPI_Send(&n, 1, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
        } else {
            MPI_Send(&n, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
        }

    }

    MPI_Finalize();
    return 0;
}