/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q2MatrixSummer.cu
    @Task:      CUDA program compute sums of two matrices, using different parallelism techniques.
 */

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void addMatRowThreads(int *a, int *b, int *c, int m, int n) {
    
    int id = threadIdx.x;
    for ( int i = 0; i < n; i++ ) {
        int ind = id * n + i;
        c[ind] = a[ind] + b[ind];
    }

}

__global__ void addMatColThreads(int *a, int *b, int *c, int m, int n) {
    
    int id = threadIdx.x;
    for ( int i = 0; i < m; i++ ) {
        int ind = i * n + id;
        c[ind] = a[ind] + b[ind];
    }

}

__global__ void addMatElementThread(int *a, int *b, int *c, int m, int n) {

    int ci = threadIdx.x;
    int ri = threadIdx.y;
    int id = ri * m + ci;
    c[id] = a[id] + b[id]; 

}

int main() {
    int *matA, *matB, *matC;
    int *da, *db, *dc;
    int m, n;
    printf("== Enter Dimension of Matrix A and B (m x n) ==\n");
    printf("m >> "); scanf("%d", &m);
    printf("n >> "); scanf("%d", &n);

    matA = (int*)malloc(sizeof(int) * m * n);
    matB = (int*)malloc(sizeof(int) * m * n);
    matC = (int*)malloc(sizeof(int) * m * n);

    printf("== Matrix A Elements ==\n");
    for(int i = 0; i < m * n; i++) {
        scanf("%d", &matA[i]);
    }
    printf("== Matrix B Elements ==\n");
    for(int i = 0; i < m * n; i++) {
        scanf("%d", &matB[i]);
    }

    cudaMalloc((void **) &da, sizeof(int) * m * n);
    cudaMalloc((void **) &db, sizeof(int) * m * n);
    cudaMalloc((void **) &dc, sizeof(int) * m * n);

    cudaMemcpy(da, matA, sizeof(int) * m * n, cudaMemcpyHostToDevice);
    cudaMemcpy(db, matB, sizeof(int) * m * n, cudaMemcpyHostToDevice);

    printf("\nChoose a degree of parallelism >> \n");
    printf("1. Thread handles row\n");
    printf("2. Thread handles column\n");
    printf("3. Thread handles element\nChoice >> ");
    int choice = 0;
    scanf("%d", &choice);
    dim3 block_conf (n, m);
    switch(choice) {
        case 1://Part A: 1 Thread handles 1 row >>
            printf("Chose: Thread handles row\n");
            addMatRowThreads<<<1,m>>>(da, db, dc, m, n);
            break;

        case 2://Part B: 1 Thread handles 1 column >>
            printf("Chose: Thread handles column\n");
            addMatColThreads<<<1,n>>>(da, db, dc, m, n);
            break;

        case 3://Part C: 1 Thread handles 1 element >>
            printf("Chose: Thread handles element\n");
            addMatElementThread<<<1, block_conf>>>(da, db, dc, m, n);
            break;

        default: 
            printf("Bad Option, exiting ...\n");
            exit(EXIT_FAILURE);
            break;
    }

    cudaMemcpy(matC, dc, sizeof(int) * m * n, cudaMemcpyDeviceToHost);
    printf("== Matrix C Elements (computed by choice %d)==\n", choice);
    for ( int i = 0; i < m; i++ ) {
        for ( int j = 0; j < n; j++ ) {
            printf("%d ", matC[i * n + j]);
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