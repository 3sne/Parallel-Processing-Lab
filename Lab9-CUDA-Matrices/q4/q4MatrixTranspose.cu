/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q4MatrixTranspose.cu
    @Task:      CUDA program compute transpose of a matrix parallely.
 */

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void transpose(int *a, int *b, int m, int n) {
    /*
        Generates b: transpose of a
    */
    int ci = threadIdx.x;
    int ri = threadIdx.y;
    b[ci * m + ri] = a[ri * n + ci];
}

int main() {
    
    int *matA, *matB;
    int *da, *db;
    int m, n;
    printf("== Enter Dimension of Matrix A (m x n) ==\n");
    printf("m >> "); scanf("%d", &m);
    printf("n >> "); scanf("%d", &n);
    matA = (int*)malloc(sizeof(int) * m * n);
    matB = (int*)malloc(sizeof(int) * m * n);
    printf("== Matrix A Elements ==\n");
    for(int i = 0; i < m * n; i++) {
        scanf("%d", &matA[i]);
    }

    cudaMalloc((void **)&da, sizeof(int) * m * n);
    cudaMalloc((void **)&db, sizeof(int) * m * n);

    cudaMemcpy(da, matA, sizeof(int) * m * n, cudaMemcpyHostToDevice);
    dim3 block_conf (n, m);
    transpose<<<1, block_conf>>>(da, db, m, n);
    cudaMemcpy(matB, db, sizeof(int) * m * n, cudaMemcpyDeviceToHost);

    printf("== Matrix B Elements ==\n");
    for ( int i = 0; i < n; i++ ) {
        for ( int j = 0; j < m; j++ ) {
            printf("%d ", matB[i * m + j]);
        }
        printf("\n");
    }

    cudaFree(da);
    cudaFree(db);
    free(matA);
    free(matB);
    return 0;
}