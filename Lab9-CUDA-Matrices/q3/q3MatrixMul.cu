/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q3MatrixMul.cu
    @Task:      CUDA program computes product of two matrices, using different parallelism techniques.
 */

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void MatMulRowThreads(int *a, int *b, int *c, int m, int n, int q) {
    
    int id = threadIdx.x; //row id
    for ( int i = 0; i < q; i++ ) {
        c[id * q + i] = 0;
        for ( int k = 0; k < n; k++ ) {
            c[id * q + i] += a[id * n + k] * b[k * q + i];
        }
    }

}

__global__ void MatMulColThreads(int *a, int *b, int *c, int m, int n, int q) {
    
    int id = threadIdx.x; //column id
    for ( int i = 0; i < m; i++ ) {
        c[i * q + id] = 0;
        for ( int k = 0; k < n; k++ ) {
            c[i * q + id] += a[i * m + k] * b[k * q + id];
        }
    }

}
 
__global__ void MatMulElementThread(int *a, int *b, int *c, int m, int n, int q) {

    int ci = threadIdx.x;
    int ri = threadIdx.y;
    int id = ri * m + ci;
    c[id] = 0;
    for ( int k = 0; k < n; k++ ) {
        c[id] += a[ri * n + k] * b[k * q + ci];
    }
}
 
int main() {
    int *matA, *matB, *matC;
    int *da, *db, *dc;
    int m, n, p, q;
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

    printf("\nChoose a degree of parallelism >> \n");
    printf("1. Thread handles row\n");
    printf("2. Thread handles column\n");
    printf("3. Thread handles element\nChoice >> ");
    int choice = 0;
    scanf("%d", &choice);
    dim3 block_conf (q, m);
    switch(choice) {
        case 1://Part A: 1 Thread handles 1 row >>
            printf("Chose: Thread handles row\n");
            MatMulRowThreads<<<1,m>>>(da, db, dc, m, n, q);
            break;

        case 2://Part B: 1 Thread handles 1 column >>
            printf("Chose: Thread handles column\n");
            MatMulColThreads<<<1,q>>>(da, db, dc, m, n, q);
            break;

        case 3://Part C: 1 Thread handles 1 element >>
            printf("Chose: Thread handles element\n");
            MatMulElementThread<<<1, block_conf>>>(da, db, dc, m, n, q);
            break;

        default: 
            printf("Bad Option, exiting ...\n");
            exit(EXIT_FAILURE);
            break;
    }

    cudaMemcpy(matC, dc, sizeof(int) * m * n, cudaMemcpyDeviceToHost);
    printf("== Matrix C Elements (computed by choice %d)==\n", choice);
    for ( int i = 0; i < m; i++ ) {
        for ( int j = 0; j < q; j++ ) {
            printf("%d ", matC[i * q + j]);
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