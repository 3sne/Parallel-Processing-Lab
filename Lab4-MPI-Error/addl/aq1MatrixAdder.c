/*
* aq1MatrixAdder.c
*
*  Created on: 29-Jan-2019
*      Author: student
*
*   Adds two 3 x 3 Matrices.	
*/


#include<mpi.h>
#include<stdio.h>

int a[3][3], b[3][3], c[3][3], myAScope[3], myBScope[3];

int main(int argc, char *argv[]) {
   int rank, size;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   if ( rank == 0) {
       printf("Enter a 3 x 3 matrix A:\n");
       for(int i = 0; i < 3; i++) {
           for(int j = 0; j < 3; j++) {
               scanf("%d", &a[i][j]);
           }
       }
        printf("Enter a 3 x 3 matrix B:\n");
       for(int i = 0; i < 3; i++) {
           for(int j = 0; j < 3; j++) {
               scanf("%d", &a[i][j]);
           }
       }
   }
   MPI_Scatter(a, 3, MPI_INT, myAScope, 3, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Scatter(b, 3, MPI_INT, myBScope, 3, MPI_INT, 0, MPI_COMM_WORLD);
   //now each process searches for the value
    int mySumRow[3];
    for (int i = 0; i < 3; i++ ) {
        mySumRow[i] = myAScope[i] + myBScope[i];
    }

    if ( rank == 0 ) {
        printf("");
    }

    MPI_Barrier(MPI_COMM_WORLD);
    for ( int i = 0; i < 3; i++ ) {
        printf("%d ", mySumRow[i]);
    }
    printf("\n");
    MPI_Finalize();
    return 0;
}
