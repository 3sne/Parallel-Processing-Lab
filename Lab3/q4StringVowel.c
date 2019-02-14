/* Finds number of vowels and consonants in a program using mpi */
#include <stdio.h>
#include <mpi.h>
#include <string.h>

int isConsonant(char c) {
    if ( c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == ' ') {
        return 0;
    } else {
        return 1;
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    char s[256];
    int n;
    int processLoad;
    int consonantCount = 0;
    int consonantData[1];
    int consonantSum[1];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter a string >> ");
        scanf("%s", s);
        n = strlen(s);
        printf("String read as >> %s\n", s);
        if (n % size != 0) {
            printf("The string length should be divisible by number of processes!\n");
            for (int i = 0; i < size; i++) {
                return 0;
            }
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(s, n, MPI_CHAR, 0, MPI_COMM_WORLD);

    processLoad = n / size;
    int start = rank * processLoad;
    int end = (rank * processLoad) + processLoad;
    for (int i = start; i < end; i++) {
        if ( isConsonant(s[i]) == 1 ) {
            printf("[Rank %d] Got consonant: %c\n", rank, s[i]);
            consonantCount += 1;
        }
    }
    consonantData[0] = consonantCount;
    MPI_Reduce(consonantData, consonantSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if ( rank == 0 ) {
        printf("Total number of consonants are %d.\n", consonantSum[0]);
    }

    MPI_Finalize();
    return 0;
}
