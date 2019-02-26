/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q1BlockSizeN.cu
    @Task:      CUDA program to add two vectors parallely with N blocks but only 1 thread per block.
 */

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
 
__global__ void add (int *a, int *b, int *c) {
    /*
        Adds Vectors a and b, puts result in c. 
    */
    int id = blockIdx.x;
    c[id] = a[id] + b[id];
}
 
int main() {
    int LS = 10240;
    int *a = (int*)malloc(sizeof(int) * LS);
    int *b = (int*)malloc(sizeof(int) * LS);
    int *c = (int*)malloc(sizeof(int) * LS);
    for (int i = 0; i < LS; i++ ) {
        a[i] = rand() % 99 + 1;
        b[i] = rand() % 99 + 1;
        c[i] = 0;
    }

    int *d_a, *d_b, *d_c;
    cudaMalloc((void **)&d_a, sizeof(int) * LS);
    cudaMalloc((void **)&d_b, sizeof(int) * LS);
    cudaMalloc((void **)&d_c, sizeof(int) * LS);

    cudaMemcpy(d_a, a, sizeof(int) * LS, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, sizeof(int) * LS, cudaMemcpyHostToDevice);

    add<<<LS,1>>>(d_a, d_b, d_c);

    cudaMemcpy(c, d_c, sizeof(int) * LS, cudaMemcpyDeviceToHost);

    for (int i = 0; i < LS; i++ ) {
        printf("%4d  + %4d == %4d\n", a[i], b[i], c[i]);
    }

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    free(a);
    free(b);
    free(c);

    return 0;
}