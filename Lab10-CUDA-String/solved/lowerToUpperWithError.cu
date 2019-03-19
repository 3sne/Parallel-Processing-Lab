#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#define n 1024


__global__ void sc(char *a, char c[n]) {
    int i = threadIdx.x;
    c[i] = (char)((int)a[i] - 32);
    // printf("%s\n", c[i]);
}

int main() {
    char a[n], c[n], *pa, *pc;
    for (int i = 0; i < n; i++) {
        a[i] = 'a';
    }
    printf("C == \n");
    
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start, 0);
    
    cudaMalloc((void**)&pa, n * sizeof(char));
    cudaMalloc((void**)&pc, n * sizeof(char));
    cudaMemcpy(pa, a, n * sizeof(char), cudaMemcpyHostToDevice);
    
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("Err1: %s\n", cudaGetErrorString(err));
    }

    sc<<<1, n>>>(pa, pc);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("Err2: %s\n", cudaGetErrorString(err));
    }

    cudaMemcpy(c, pc, n * sizeof(char), cudaMemcpyDeviceToHost);
    
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    float et;
    cudaEventElapsedTime(&et, start, stop);

    for (int i = 0; i < n; i++) {
        printf("%c -> %c\n", a[i], c[i]);
    }
    printf("Kernel exec time on device : %f\n", et);
    cudaFree(pa);
    cudaFree(pc);

}