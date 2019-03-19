/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q1FindSubstring.cu
    @Task:      CUDA program that finds a substring in a given string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

void resetBuf(char* b, int blen) {
    for ( int i = 0; i < blen; i++ )
        b[i] = '\0';
}

__global__ void findSs(char *text, char *ss, int *retArr, int ssLen) {
    int tid = threadIdx.x;
    int w = 0;
    retArr[tid] = 1; 
    for (int i = tid; i < (tid + ssLen); i++ ) {
        if (text[i] != ss[w]) {
            retArr[tid] = 0;
            break;
        }
        w += 1;
    }
}

int main() {
    char *buf = (char*)malloc(sizeof(char) * 10240);
    char *text, *subStr, *dtext, *dsubStr;
    int  *retArr, *dretarr;

    printf("[IN] Enter Text >> ");
    scanf("%[^\n]s", buf);
    int tLen = strlen(buf);
    text = (char*)malloc(sizeof(char) * tLen);
    strcpy(text, buf);

    resetBuf(buf, 10240);

    printf("[IN] Enter Sub-String >> ");
    scanf("%s", buf);
    int ssLen = strlen(buf);
    subStr = (char*)malloc(sizeof(char) * ssLen);
    strcpy(subStr, buf);
    free(buf);

    retArr = (int*)malloc(sizeof(int) * (tLen - ssLen + 1));
    for (int i = 0; i < (tLen - ssLen + 1); i++) {
        retArr[i] = 0;
    }

    cudaMalloc((void **)&dtext, sizeof(char) * tLen);
    cudaMalloc((void **)&dsubStr, sizeof(char) * ssLen);
    cudaMalloc((void **)&dretarr, sizeof(int) * (tLen - ssLen + 1));

    cudaMemcpy(dtext, text, sizeof(char) * tLen, cudaMemcpyHostToDevice);
    cudaMemcpy(dsubStr, subStr, sizeof(char) * ssLen, cudaMemcpyHostToDevice);
    cudaMemcpy(dretarr, retArr, sizeof(int) * (tLen - ssLen + 1), cudaMemcpyHostToDevice);
    dim3 block_conf (tLen - ssLen + 1, 1);
    findSs<<<1, block_conf>>>(dtext, dsubStr, dretarr, ssLen);
    cudaMemcpy(retArr, dretarr, sizeof(int) * (tLen - ssLen + 1), cudaMemcpyDeviceToHost);
    
    int yay = 0;
    for (int i = 0; i < tLen - ssLen + 1; i++) {
        if (retArr[i] == 1) {
            yay = 1;
            break;
        }
    }

    if (yay) {
        printf("Substring found in Text @ index(es) ");
        for (int i = 0; i < tLen - ssLen + 1; i++) {
            if (retArr[i])
                printf("%d ", i);
        }
    } else {
        printf("Substring un-found in Text :(");
    }
    printf("\n");

    cudaFree(dtext);
    cudaFree(dsubStr);
    cudaFree(dretarr);
    free(text);
    free(subStr);
    return 0;
}