/*
* q1FactSum.c
*
*  Created on: 29-Jan-2019
*      Author: student
*
*	Generates sum of factorials in parallel using mpi
*/


#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
   int rank, size;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   int n = size, answer;
   int fact = 1;
   for ( int i = 1; i <= rank + 1; i++ ) {
       fact *= i;
   }
   printf("[%d] Fact is %d\n", rank, fact);
   MPI_Scan(&fact, &answer, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
   if ( rank == size - 1 ) {
       printf("The sum of factorials is %d\n", answer);
   }
   MPI_Finalize();
   return 0;
}
