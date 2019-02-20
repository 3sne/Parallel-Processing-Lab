/* Concatenates (acutally intertwines) two strings using mpi */

#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rank, size;
    char s1[256];
    char s2[256];
    char temp[256];
    int n;
    int processLoad;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter String 1 >> ");
        scanf("%s", s1);
        printf("Enter String 2 >> ");
        scanf("%s", s2);
        n = strlen(s1);
        int temp = strlen(s2);
        printf("String1 read as >> %s\n", s1);
        printf("String2 read as >> %s\n", s2);
        
        //input filtering
        if (temp != n) {
            printf("[Rank %d] Strings must be of equal length -_- BYE.\n", rank);
            return 0;
        }
        if (n % size != 0) {
            printf("The string(s) length should be divisible by number of processes!\n");
            return 0;
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(s1, n, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(s2, n, MPI_CHAR, 0, MPI_COMM_WORLD);

    processLoad = n / size;
    int start = rank * processLoad;
    int end = (rank * processLoad) + processLoad;
    int cnt = 0;
    for (int i = start; i < end; i++) {
        temp[cnt++] = s1[i];
        temp[cnt++] = s2[i];
    }
    printf("[Rank %d] %s\n", rank, temp);
    MPI_Gather(temp, processLoad * 2, MPI_CHAR, &s1, processLoad * 2, MPI_CHAR, 0, MPI_COMM_WORLD);

    if( rank == 0 ) {
        printf("Final concatenated string is \"%s\"\n", s1);
    }

    MPI_Finalize();
    return 0;
}
