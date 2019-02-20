/*
* q3SearchElement.c
*
*  Created on: 29-Jan-2019
*      Author: student
*
*	Searches for total number of instances of the given search parameter.
*/


#include<mpi.h>
#include<stdio.h>

int a[3][3], mySearchScope[3], searchMe, result = 0;

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
       printf("Enter the item to be searched >> ");
       scanf("%d", &searchMe);
   }
   MPI_Scatter(a, 3, MPI_INT, mySearchScope, 3, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&searchMe, 1, MPI_INT, 0, MPI_COMM_WORLD);

   //now each process searches for the value
   int instancesInMe = 0;
   for (int i = 0; i < 3; i++ ) {
       if ( mySearchScope[i] == searchMe ) {
           instancesInMe += 1;
       }
   }
   MPI_Reduce(&instancesInMe, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   if ( rank == 0 ) {
       if ( result > 0 ) {
           printf("%d was found in the matrix %d times!\n", searchMe, result);
       } else {
           printf("Couldn't find %d in the matrix!\n", searchMe);
       }
   }
   MPI_Finalize();
   return 0;
}
