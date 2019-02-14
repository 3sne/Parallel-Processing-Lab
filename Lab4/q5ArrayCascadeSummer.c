/*
* q5ArrayCascadeSummer.c
*
*  Created on: 29-Jan-2019
*      Author: student
*
*  Takes a 4 x 4 Matrix and generates fancy column cascade sum matrix
*/


#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[])
{
   int rank, size;
   int a[4][4], col[4], b[4];
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   if (rank == 0) {
       printf("Enter 4x4 matrix:\n");
       for(int i = 0; i < 4; i++) {
           for(int j = 0; j < 4; j++) {
               scanf("%d", &a[i][j]);
           }
       }
   }
   MPI_Scatter(a, 4, MPI_INT, b, 4, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Scan(b, col, 4, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

   MPI_Barrier(MPI_COMM_WORLD);

   if(rank == 0 ) {
       printf("Output matrix: \n");
   }

   for(int i = 0; i < 4; i++) {
       printf("%d ", col[i]);
   }
   printf("\n");

   MPI_Finalize();
   return 0;
}
