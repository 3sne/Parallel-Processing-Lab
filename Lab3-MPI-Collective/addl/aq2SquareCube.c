/* takes elements of array in batches of m. Squares, Cubes etc depending on batch number using mpi */
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
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
    }
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(a, m, MPI_INT, catcher, m, MPI_INT, 0, MPI_COMM_WORLD);
    int op = rank + 1;
    for ( int i = 0; i < m; i++ ) {
        for ( int x = op; x > 0; x-- ) {
            catcher[i] *= catcher[i];
        }
    }
    printf("Rank%d : Me done this (Degree %d) >> ", rank, rank + 2);
    for ( int i = 0; i < m; i++ ) {
        printf("%d ", catcher[i]);
    }
    printf("\n");
    MPI_Finalize();
    return 0;
}
