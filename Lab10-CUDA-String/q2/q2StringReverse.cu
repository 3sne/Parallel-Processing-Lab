/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q2StringReverse.cu
    @Task:      CUDA program that the reverses given string.
 */

#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void reverse(char *str, char *rev, int len) {
	int tid = threadIdx.x;
	rev[len - tid - 1] = str[tid];
}

int main() {
	char *dstr, *drev;
	char str[256], rev[256];
	printf("Enter the string >> ");
	scanf("%s", str);
	int len = strlen(str);
	
	cudaMalloc((void **)&dstr, len * sizeof(char));
	cudaMalloc((void **)&drev, len * sizeof(char));
	
	cudaMemcpy(dstr, str, len * sizeof(char), cudaMemcpyHostToDevice);
	reverse<<<1, len>>>(dstr, drev, len);
	cudaMemcpy(rev, drev, len * sizeof(char), cudaMemcpyDeviceToHost);

	printf("Reverse: %s\n", rev);
}
