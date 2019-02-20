/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q4SuperSum.c
    @Task:      OpenCL Program to replace element by sum of row-sum and column-sum of where it belongs in a matrix.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include <time.h>
#define MSS (0x100000)

void eprint(char *s, cl_int ec) {
    if (ec != CL_SUCCESS) {
        printf("ERROR: %d | %s\n", ec, s);
    }
}

int main(int argc, char const *argv[]) {
    int i;
    clock_t start, end;
    start = clock();

  //user input
    int nRow, nCol;
    printf("=====Matrix A (m x n)=====\n");
    printf("Rows (m)    >> ");
    scanf("%d", &nRow);
    printf("Columns (n) >> ");
    scanf("%d", &nCol);
    int *matA = (int *)malloc(sizeof(int) * nRow * nCol);
    int *matB = (int *)malloc(sizeof(int) * nRow * nCol);
    printf("Elements    >> \n");
    for ( int i = 0; i < nRow * nCol; i++ ) {
        scanf("%d", &matA[i]);
    }

  //kernel src load
    FILE *f;
    char *srcStr;
    size_t SS;
    f = fopen("q4.cl", "r");
    if ( !f ) {
        fprintf(stderr, "KERNEL LOAD FAIL [0]\n");
        getchar();
        exit(EXIT_FAILURE);
    }
    srcStr = (char*)malloc(MSS);
    SS = fread(srcStr, 1, MSS, f);
    fclose(f);

  //inits
    cl_platform_id      platid = NULL;
    cl_device_id        deviceid = NULL;
    cl_uint             numDevices, numPlats;
    cl_int              ret;
    cl_context          context;
    cl_command_queue    cmdq;
    cl_program          program;
    cl_kernel           kernel;
    cl_event            event;
    cl_ulong            time_start;
    cl_ulong            time_end;
    double              tot_time;


    //GO
    ret = clGetPlatformIDs(1, &platid, &numPlats);
    eprint("plat",ret);
    ret = clGetDeviceIDs(platid, CL_DEVICE_TYPE_ALL, 1, &deviceid, &numDevices);
    eprint("dev", ret);
    context = clCreateContext(NULL, 1, &deviceid, NULL, NULL, &ret);
    cmdq = clCreateCommandQueue(context, deviceid, CL_QUEUE_PROFILING_ENABLE, &ret);
    
    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, nRow * nCol * sizeof(int), NULL, &ret);
    eprint("amemobj", ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, nRow * nCol * sizeof(int), NULL, &ret);
    eprint("bmemobj", ret);

    ret = clEnqueueWriteBuffer(cmdq, a_mem_obj, CL_TRUE, 0, nRow * nCol * sizeof(int), matA, 0, NULL, NULL);
    eprint("amemobj Write", ret);

    program = clCreateProgramWithSource(context, 1, (const char**) &srcStr, (const size_t*)&SS, &ret);
    eprint("prog src", ret);
    ret = clBuildProgram(program, 1, &deviceid, NULL, NULL, NULL);
    eprint("build prog", ret);
    kernel = clCreateKernel(program, "superSummer", &ret);
    eprint("create kernel", ret);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &a_mem_obj);
    eprint("kernel arg 0", ret);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &b_mem_obj);
    eprint("kernel arg 1", ret);

    size_t global_item_size[2]  = {nCol, nRow};
    size_t local_item_size[2]   = {   1,    1};
    ret = clEnqueueNDRangeKernel(cmdq, kernel, 2, NULL, global_item_size, local_item_size, 0, NULL, &event);
    ret = clFinish(cmdq);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    tot_time = (double)(time_end - time_start);
    ret = clEnqueueReadBuffer(cmdq, b_mem_obj, CL_TRUE, 0, nRow * nCol * sizeof(int), matB, 0, NULL, NULL);
    eprint("read out", ret);
    
    //display result
    printf("\n======= Matrix A =======\n");
    for ( int i = 0; i < nRow * nCol; i++ ) {
        if(i % nCol == 0) {
            printf("\n ");
        }
        printf("%5d ", matA[i]);
    }
    printf("\n");
    printf("\n======= Matrix B =======\n");
    for ( int i = 0; i < nRow * nCol; i++ ) {
        if(i % nCol == 0) {
            printf("\n ");
        }
        printf("%5d ", matB[i]);
    }
    printf("\n");

    //cleanup
    ret = clFlush(cmdq);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseCommandQueue(cmdq);
    ret = clReleaseContext(context);
    free(matA);
    

    end = clock();
    printf("\n=============================TIME INFO=============================\n");
    printf("Time for kernel execution >> %20.20fs\n", tot_time / 1000000);
    printf("Time for driver execution >> %20.20fs\n", (end - start) / (double)CLOCKS_PER_SEC);
    return 0;
}
