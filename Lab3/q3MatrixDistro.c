/* Calculates row-wise avg of matrix elements using mpi*/

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int rank, size, n, m, gotRow[256], a[1024];
    float avgCollection[256];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    n = size;
    

    if (rank == 0) {
        printf("Enter m (n is %d) >> ", n);
        scanf("%d", &m);
        printf("n -> %d | m -> %d\n", n, m);
        printf("Enter the n x m array >> \n");
        for (int i = 0; i < n * m; i++) {
            scanf("%d", &a[i]);
        }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(a, m, MPI_INT, gotRow, m, MPI_INT, 0, MPI_COMM_WORLD);

    float localAvg = 0;
    for ( int i = 0; i < m; i++) {
        localAvg += gotRow[i];
    }
    localAvg /= m;
    
    MPI_Gather(&localAvg, 1, MPI_FLOAT, avgCollection, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if ( rank == 0 ) {
        float finalAvg = 0;
        for ( int i = 0; i < n; i++) {
            finalAvg += avgCollection[i];
        }
        finalAvg /= n;
        printf("[Rank %d] Average of all the elements is >> %.2f\n", rank, finalAvg);
    }
    MPI_Finalize();
    return 0;
}