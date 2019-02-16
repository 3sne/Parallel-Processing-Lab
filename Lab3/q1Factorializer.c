/* Prints factorials of elements in an array using mpi */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int rank, size, n, a[256], gotNumber;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        printf("How many numbers you wanna give? ");
        scanf("%d", &n);
        printf("Enter the numbers >> ");
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &a[i]);
        }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);   
    MPI_Scatter(a, 1, MPI_INT, &gotNumber, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int f = 1;
    printf("Rank %d got number %d\n", rank, gotNumber);
    for (int i = 1; i <= gotNumber; i++ ) {
        f = f * i;
    }
    printf("Rank %d Fact is %d\n", rank, f);
    MPI_Finalize();
    return 0;
}