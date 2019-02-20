/*
 * q5MpiPrimeList.c
 *
 *  Created on: 08-Jan-2019
 *      Author: student
 */
/*
 * q3MpiCalculator.c
 *
 *  Created on: 08-Jan-2019
 *      Author: student
 */
#include <stdio.h>
#include <mpi.h>

int rank, size;

void primify(int l, int h) {
	int f = 0;
	for ( int i = l; i <= h; i++ ) {
		f = 1;
		for ( int j = 2; j < i / 2; j++ ) {
			if ( i % j == 0 ) {
				f = 0;
				break;
			}
		}
		if ( f == 1) {
			printf("[Rank %d] %d\n", rank, i);
		}
	}
}

int main(int argc, char *argv[]) {
	int a = 10, b = 3;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if( rank == 0 ) { //find in 1 - 50
		primify(1, 50);
	} else if( rank == 1 ) { //find in 51 - 100
		primify(51, 100);
	} else {
		printf("[Rank %d]Not Assigned Work to me :(\n", rank);
	}

	MPI_Finalize();
	return 0;
}
