/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q4MatAddAndMul2D.cu
    @Task:      CUDA program that calculates multiplication and addition of two matrices using 2D Grid & 2D Block.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cuda_runtime.h>

__device__ int getTid() {
    int blockSkip = (blockIdx.y * gridDim.x * blockDim.x * blockDim.y); 
    int rowSkip = (threadIdx.y * gridDim.x * blockDim.x);
    int rowDisplacement = (blockIdx.x * blockDim.x) + threadIdx.x;
    int tid = blockSkip + rowSkip + rowDisplacement;
    return tid;
}

__global__ void MatAddElementThread(int *a, int *b, int *d) {
    int tid = getTid();
    d[tid] = a[tid] + b[tid];
}

__global__ void MatMulElementThread(int *a, int *b, int *c, int n, int q) {
    int tid = getTid();
    int initDisp = tid % q;
    c[tid] = 0;
    for (int k = 0; k < n; k++) {
        c[tid] += a[tid - initDisp + k] * b[k * q + initDisp];
    }
}

int main() {
    system("clear");

    int *matA, *matB, *matProd, *matSum;
    int *da, *db, *dc, *dd;
    int m, n, p, q;
    int willMul = 1, willAdd = 1;
    
    printf("[NOTE] Both Matrices should have SAME and EVEN dimensions to successfully compute both the sum and the product (i.e, EVEN SQUARE MATRICES)\n");
    printf("\n== Enter Dimension of Matrix A (m x n) ==\n");
    printf("m >> "); scanf("%d", &m);
    printf("n >> "); scanf("%d", &n);
    matA = (int*)malloc(sizeof(int) * m * n);
    printf("== Matrix A Elements ==\n");
    for(int i = 0; i < m * n; i++) {
        scanf("%d", &matA[i]);
    }

    printf("\n== Enter Dimension of Matrix B (p x q) ==\n");
    printf("p >> "); scanf("%d", &p);
    printf("q >> "); scanf("%d", &q);
    if ( m % 2 || n % 2 || p % 2 || q % 2) {
        free(matA);
        printf("[PROGRAM] You don't listen to me, do you? I told you O N L Y  E V E N dimensions.....\n"); sleep(2);
        printf("[PROGRAM] Why this restriction? Because I use a fixed block size of (2, 2). For odd dimensions, I don't like keeping some threads lonely. Thread lives matter. /\n"); sleep(2);
        printf("[PROGRAM] But you clearly don't care, so don't I. Byeee Noob \\(^.^)\n"); sleep(2);
        exit(EXIT_FAILURE);
    }
    if (n != p) {
        willMul = 0;
        printf("[MUL ERROR] n & p must be equal, Skipping Matrix Multiplication...\n"); sleep(1);
    }
    if (m != p || n != q) {
        willAdd = 0;
        printf("[ADD ERROR] Dimensions of matA and matB are unequal, skipping Matrix Addition...\n"); sleep(1);
    }
    matB = (int*)malloc(sizeof(int) * p * q);
    printf("== Matrix B Elements ==\n");
    for(int i = 0; i < p * q; i++) {
        scanf("%d", &matB[i]);
    }

    matProd = (int*)malloc(sizeof(int) * m * q);
    matSum  = (int*)malloc(sizeof(int) * m * n);

    cudaMalloc((void **) &da, sizeof(int) * m * n);
    cudaMalloc((void **) &db, sizeof(int) * p * q);
    cudaMalloc((void **) &dc, sizeof(int) * m * q);
    cudaMalloc((void **) &dd, sizeof(int) * m * n);

    cudaMemcpy(da, matA, sizeof(int) * m * n, cudaMemcpyHostToDevice);
    cudaMemcpy(db, matB, sizeof(int) * p * q, cudaMemcpyHostToDevice);

    dim3 grid_conf  (q / 2, m / 2);
    dim3 block_conf (2, 2);
    
    if (willMul) {   
        MatMulElementThread<<<grid_conf, block_conf>>>(da, db, dc, n, q);
        cudaMemcpy(matProd, dc, sizeof(int) * m * q, cudaMemcpyDeviceToHost);
        printf("\n-=Result of Multiplication=-\n");
        printf("----------------------------\n");
        for (int i = 0; i < m; i++ ) {
            for (int j = 0; j < q; j++) {
                printf("%6d ", matProd[i * q + j]);
            }
            printf("\n");
        }
    }

    if (willAdd) {
        MatAddElementThread<<<grid_conf, block_conf>>>(da, db, dd);
        cudaMemcpy(matSum, dd, sizeof(int) * m * n, cudaMemcpyDeviceToHost);
        printf("\n-=Result of Addition=-\n");
        printf("----------------------\n");
        for (int i = 0; i < m; i++ ) {
            for (int j = 0; j < n; j++) {
                printf("%6d ", matSum[i * n + j]);
            }
            printf("\n");
        }
    }
    
    if (!willAdd && !willMul) {
        printf("Bad Matrix dimensions, exiting...\n");
    }

    printf("\n");
    cudaFree(da);
    cudaFree(db);
    cudaFree(dc);
    cudaFree(dd);
    free(matA);
    free(matB);
    free(matProd);
    free(matSum);
    return 0;
}