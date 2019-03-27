/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q4MatAddAndMul2D.cu
    @Task:      CUDA program that calculates multiplication and addition of two matrices using 2D Grid & 2D Block.
 */

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__device__ int getTid() {
	int blockSkip = (blockIdx.y * gridDim.x * blockDim.x * blockDim.y); 
	int rowSkip = (threadIdx.y * gridDim.x * blockDim.x);
	int rowDisplacement = (blockIdx.x * blockDim.x) + threadIdx.x;
	int tid = blockSkip + rowSkip + rowDisplacement;
	return tid;
}

__global__ void MatMulElementThread(int *a, int *b, int *c, int q) {
	int tid = getTid();
	int initDisp = tid % q;
	c[tid] = 0;
	for (int k = 0; k < n; k++) {
		c[tid] += a[tid - initDisp + k] * b[k * q + initDisp];
	}
}

int main() {
	int *matA, *matB, *matC;
	int *da, *db, *dc;
	int m, n, p, q;
	printf("ALL DIMENSIONS MUST BE EVEN\n");
	printf("== Enter Dimension of Matrix A (m x n) ==\n");
	printf("m >> "); scanf("%d", &m);
	printf("n >> "); scanf("%d", &n);
	matA = (int*)malloc(sizeof(int) * m * n);
	printf("== Matrix A Elements ==\n");
	for(int i = 0; i < m * n; i++) {
		scanf("%d", &matA[i]);
	}

	printf("== Enter Dimension of Matrix B (p x q) ==\n");
	printf("p >> "); scanf("%d", &p);
	printf("q >> "); scanf("%d", &q);
	matB = (int*)malloc(sizeof(int) * p * q);
	if ( n != p ) {
		printf("[ERROR] n & p must be equal, Exiting ...\n");
		exit(EXIT_FAILURE);
	}
	printf("== Matrix B Elements ==\n");
	for(int i = 0; i < p * q; i++) {
		scanf("%d", &matB[i]);
	}

	matC = (int*)malloc(sizeof(int) * m * q);

	cudaMalloc((void **) &da, sizeof(int) * m * n);
	cudaMalloc((void **) &db, sizeof(int) * p * q);
	cudaMalloc((void **) &dc, sizeof(int) * m * q);

	cudaMemcpy(da, matA, sizeof(int) * m * n, cudaMemcpyHostToDevice);
	cudaMemcpy(db, matB, sizeof(int) * p * q, cudaMemcpyHostToDevice);

	dim3 grid_conf  (q / 2, m / 2);
	dim3 block_conf (2, 2);
	MatMulElementThread<<<grid_conf, block_conf>>>(da, db, dc, q);
	cudaMemcpy(matC, dc, sizeof(int) * m * q, cudaMemcpyDeviceToHost);

	for (int i = 0; i < m; i++ ) {
		for (int j = 0; j < q; j++) {
			printf("%3d ", matC[i * q + j]);
		}
		printf("\n");
	}

	cudaFree(da);
	cudaFree(db);
	cudaFree(dc);
	free(matA);
	free(matB);
	free(matC);
	return 0;
}