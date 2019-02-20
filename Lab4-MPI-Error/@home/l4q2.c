#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    double PI50 = 3.14159265358979323846264338327950288419716939937510;
    double partPi, pi, h, sum, x, a;
    int n, rc;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if ( rank == 0 ) {
        printf("Enter Intervals >> ");
        scanf("%d", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    h = 1.0 / (double)n;
    sum = 0.0;
    for(int i = rank + 1; i <= n; i +=size ) {
        x = h * ((double)i - 0.5);
        sum += 4 / (1.0 + x * x);
    }
    partPi = h * sum;
    MPI_Reduce(&partPi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if ( rank == 0) {
        printf("%1.50f :: Real PI\n", PI50);
        printf("%1.50f :: Calculated PI\n", pi);
        printf("%1.50f :: Difference\n", PI50 - pi);
    }
    MPI_Finalize();
    return 0;
}
