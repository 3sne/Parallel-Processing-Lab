#include <mpi.h>
#include <stdio.h>

int a[256][256], b[256][256], c[256][256], m, n, p;

int main(int argc, char *argv[]) {

	// MPI_Init(&argc, &argv);

	printf("Enter m, n and p >> ");
	scanf("%d%d%d", &m, &n, &p);

	printf("Enter mat A (m x n): \n");
	for( int i = 0; i < m; i++ ) {
		for( int j = 0; j < n; j++ ) {
			scanf("%d", &a[i][j]);
		}
	}

	printf("Enter mat B (n x p): \n");
	for( int i = 0; i < m; i++ ) {
		for( int j = 0; j < p; j++ ) {
			scanf("%d", &b[i][j]);
		}
	}

	//computing matrix C = A x B

	for( int i = 0; i < m; i++ ) {
		for( int j = 0; j < p; j++ ) {
			for( int k = 0; k < n; k++ ) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	printf("mat C is: \n");
	for( int i = 0; i < m; i++ ) {
		for( int j = 0; j < p; j++ ) {
			printf("%d ", c[i][j]);
		}
		printf("\n");
	}
}
