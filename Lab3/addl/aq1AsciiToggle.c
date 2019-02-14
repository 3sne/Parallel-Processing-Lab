#include <stdio.h>
#include <mpi.h>
#include <string.h>

char toggleChar(char c) {
	if (c>= 65 && c <= 90) {
		return (char)((int)c + 32);
	} else if (c >= 97 && c <= 122 ) {
		return (char)((int)c - 32);
	} else {
		return c;
	}
}

int main(int argc, char* argv[]) {
	int rank, size;
	char s[256],temp[256];
	int n, m;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
    n = size;

	if (rank == 0) {
		printf("Enter string >> "); scanf("%s", s);
		m = strlen(s);
	}
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(s, m, MPI_CHAR, 0, MPI_COMM_WORLD);
    int processLoad = m / size;
    int start = processLoad * rank;
    int end = start + processLoad;
	int cnt = 0;
    for ( int i = start; i < end; i++) {
        temp[cnt++] = toggleChar(s[i]);
		printf("[Rank %d] %c -> %c | New Ascii >> %d\n", rank, s[i], temp[cnt - 1], (int)temp[cnt - 1]);
    }
	MPI_Gather(temp, processLoad, MPI_CHAR, s, processLoad, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
	if ( rank == 0 ) {
		printf("[Rank %d] Toggled string is %s\n", rank, s);
	}
	MPI_Finalize();
	return 0;
}
