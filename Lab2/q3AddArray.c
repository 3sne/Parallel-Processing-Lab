#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, n, a[100000];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if ( rank == 0 ) {
        
        printf("[Rank %d] Enter array size >> ", rank);
        scanf("%d", &n);
        printf("[Rank %d] Enter array >> ", rank);
        for ( int i = 0; i < n; i++ ) {
            scanf("%d", &a[i]);
        }

        for ( int i = 1; i < size; i++ ) { //send array size
           MPI_Send(&n, 1, MPI_INT, i, i, MPI_COMM_WORLD);     
        }
        for ( int i = 1; i < size; i++ ) {
            int len;
            if ( n % 2 == 0 || i == 1) {
                len = n / 2;
            } else {
                len = (n / 2) + 1;
            }
            MPI_Send(&a[(n / 2) * (i - 1)], len, MPI_INT, i, i + 2, MPI_COMM_WORLD);
        }
        int sum = 0;
        for ( int i = 1; i < size; i++ ) {
            MPI_Recv(&n, 1, MPI_INT, i, i * 4, MPI_COMM_WORLD, &status);
            sum += n;
        }
        
        printf("[Rank %d] Sum of all the elements is >> %d\n", rank, sum);

    } else {
        int len;
        MPI_Recv(&n, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
        if ( n % 2 == 0 || rank == 1) {
            len = n / 2;
        } else {
            len = (n / 2) + 1;
        }
        MPI_Recv(a, len, MPI_INT, 0, rank + 2, MPI_COMM_WORLD, &status);

        printf("[Rank %d] Got >> ", rank);
        for ( int i = 0; i < len; i++) {
            printf("%d ", a[i]);
        }
        printf("\n");

        int sum = 0;
        double start, end;
        start = MPI_Wtime();
        for ( int i = 0; i < len; i++ ) {
            sum += a[i];
        }
        end = MPI_Wtime();
        MPI_Send(&sum, 1, MPI_INT, 0, rank * 4, MPI_COMM_WORLD);
        printf("[Rank %d] Time taken for computation >> %f\n", rank, end-start);
    }

    MPI_Finalize();
    return 0;
}































// /for ( int j = (n / 2) * i; j < (n / 2) * ( i + 1 ); j++ ) {

            // }