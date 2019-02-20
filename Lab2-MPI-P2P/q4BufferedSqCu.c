#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, n, a[256];
    int BUFSIZE = 256;
    int buf[BUFSIZE];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Buffer_attach(buf, BUFSIZE);

    if ( rank == 0 ) {
        
        printf("[Rank %d] Enter array size >> ", rank);
        scanf("%d", &n);
        printf("[Rank %d] Enter array >> ", rank);
        for ( int i = 0; i < n; i++ ) {
            scanf("%d", &a[i]);
        }

        for ( int i = 1; i < size; i++ ) { //send array size
           MPI_Bsend(&a[i], 1, MPI_INT, i, i, MPI_COMM_WORLD);     
        }

    } else {
        
        MPI_Recv(&n, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
        if ( rank % 2 == 0 ) {
            printf("[Rank %d] Got >> %d | Squared >> %d\n", rank, n, n * n);
        } else {
            printf("[Rank %d] Got >> %d | Cubed >> %d\n", rank, n, n * n * n);
        }

    }

    MPI_Buffer_detach(&buf, &BUFSIZE);
    MPI_Finalize();
    return 0;
}