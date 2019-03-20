/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q3BinaryMaker.cu
    @Task:      CUDA program that converts chars of a string to binary.
 */

#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void makeBinaries(char *str, int *binArr) {
	int tid = threadIdx.x;
	int n = (int)str[tid];
	int rem, i = 1, bin = 0;
    while (n > 0) {
		rem = n % 2;
		n = n / 2;
        bin = bin + rem * i;
        i = i * 10;
    }
    binArr[tid] = bin;
}	

int main() {
	char *dStr; int *dBinArr;
	char *str = (char*)malloc(sizeof(char) * 10240);
	printf("Enter the string >> "); scanf("%[^\n]s",  str);
	int len = strlen(str);
	int *binArr = (int*)malloc(sizeof(int) * len);
	
	cudaMalloc((void **)&dStr , len*sizeof(char));
	cudaMalloc((void **)&dBinArr, len*sizeof(int));
	
	cudaMemcpy(dStr, str, len * sizeof(char), cudaMemcpyHostToDevice);
	makeBinaries<<<1, len>>>( dStr,  dBinArr);
	cudaMemcpy(binArr, dBinArr, len * sizeof(int), cudaMemcpyDeviceToHost);

	printf("Output: \n");
	for(int i = 0 ; i < len; i++) {
		printf("'%c' -> %10d\n",  str[i] ,  binArr[i]);
	}
}
