/*
 * q3MpiCalculator.c
 *
 *  Created on: 08-Jan-2019
 *      Author: student
 */
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
	int rank, size;
	int a = 10, b = 3;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	switch( rank ) {
	case 0: printf("[Rank: %d] a + b = %d [Addition]", rank, a + b);
			break;
	case 1: printf("[Rank: %d] a - b = %d [Subtraction]", rank, a - b);
				break;
	case 2: printf("[Rank: %d] a * b = %d [Multiplication]", rank, a * b);
				break;
	case 3: printf("[Rank: %d] a / b = %d [Division]", rank, a / b);
				break;
	case 4: printf("[Rank: %d] a % b = %d [Modulus]", rank, a % b);
				break;
	}

	MPI_Finalize();
	return 0;
}
