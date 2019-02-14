/* computes sum from 1 to N for a every number N in an array using mpi parallelism */
#include <stdio.h>
#include <mpi.h>

int sumFromOneTo( int num ) {
    int temp = 0;
    for ( int i = 1; i <= num; i++ ) {
        temp += i;
    }
    return temp;
}

int main(int argc, char *argv[]) {
    int rank, size, n, m;
    int a[1024];
    int catcher[256];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    n = size;
    if (rank == 0) {
        printf("Enter m >> ");
        scanf("%d", &m);
        printf("n -> %d  ||  m -> %d\n", n, m);
        printf("Enter the numbers >> ");
        for (int i = 0; i < n * m; i++ ) {
            scanf("%d", &a[i]);
        }
        printf("Array before >> ");
        for (int i = 0; i < n * m; i++ ) {
            printf("%10d ", a[i]);
        }
        printf("\n");
    }
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(a, m, MPI_INT, catcher, m, MPI_INT, 0, MPI_COMM_WORLD);
    
    for (int i = 0; i < m; i++ ) {
        catcher[i] = sumFromOneTo(catcher[i]);
    }

    MPI_Gather(catcher, m, MPI_INT, a, m, MPI_INT, 0, MPI_COMM_WORLD);

    if ( rank == 0 ) {
        printf("Array after  >> ");
        for (int i = 0; i < n * m; i++ ) {
            printf("%10d ", a[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
