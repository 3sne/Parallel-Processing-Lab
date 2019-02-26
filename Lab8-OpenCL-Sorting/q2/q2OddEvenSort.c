/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q2OddEvenSort.c
    @Task:      OpenCL program to sort a vector using odd-even transposition technique.
 */

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <time.h>
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define MSS (0x100000)

void eprint(cl_int ec) {
    if (ec != CL_SUCCESS) {
        printf("ERROR: %d\n", ec);
    }
}

int main(int argc, char const *argv[]) {
    int i;
    clock_t start, end;
    start = clock();

  //user input
    const int LS = 30;
    int *list = (int*) malloc(sizeof(int) * LS);
    printf("LIST >> ");
    for ( int i = 0; i < LS; i++ ) {
        list[i] = rand() % 999 + 1;
        printf("%d ", list[i]);
    }
    printf("\n");

  //kernel src load
    FILE *f;
    char *srcStr1, *srcStr2;
    size_t SS1, SS2;

    f = fopen("q2OddEven.cl", "r");
    if ( !f ) {
        fprintf(stderr, "KERNEL 1 LOAD FAIL [0]\n");
        getchar();
        exit(EXIT_FAILURE);
    }
    srcStr1 = (char*)malloc(MSS);
    SS1 = fread(srcStr1, 1, MSS, f);
    fclose(f);
    f = fopen("q2EvenOdd.cl", "r");
    if ( !f ) {
        fprintf(stderr, "KERNEL 2 LOAD FAIL [0]\n");
        getchar();
        exit(EXIT_FAILURE);
    }
    srcStr2 = (char*)malloc(MSS);
    SS2 = fread(srcStr2, 1, MSS, f);
    fclose(f);

  //inits
    cl_platform_id      platid = NULL;
    cl_device_id        deviceid = NULL;
    cl_uint             numDevices, numPlats;
    cl_int              ret;
    cl_context          context;
    cl_command_queue    cmdq;
    cl_program          program1, program2;
    cl_kernel           kernel1, kernel2;
    cl_event            event;
    cl_ulong            time_start;
    cl_ulong            time_end;
    double              tot_time;


    //GO
    ret = clGetPlatformIDs(1, &platid, &numPlats);
    eprint(ret);
    ret = clGetDeviceIDs(platid, CL_DEVICE_TYPE_ALL, 1, &deviceid, &numDevices);
    eprint(ret);
    context = clCreateContext(NULL, 1, &deviceid, NULL, NULL, &ret);
    eprint(ret);
    cmdq = clCreateCommandQueue(context, deviceid, CL_QUEUE_PROFILING_ENABLE, &ret);
    eprint(ret);

    cl_mem listObj = clCreateBuffer(context, CL_MEM_READ_ONLY, LS * sizeof(int), NULL, &ret);
    eprint(ret);
    ret = clEnqueueWriteBuffer(cmdq, listObj, CL_TRUE, 0, LS * sizeof(int), list, 0, NULL, NULL);
    eprint(ret);

    program1 = clCreateProgramWithSource(context, 1, (const char**) &srcStr1, (const size_t*)&SS1, &ret);
    eprint(ret);
    program2 = clCreateProgramWithSource(context, 1, (const char**) &srcStr2, (const size_t*)&SS2, &ret);
    eprint(ret);

    ret = clBuildProgram(program1, 1, &deviceid, NULL, NULL, NULL);
    eprint(ret);
    ret = clBuildProgram(program2, 1, &deviceid, NULL, NULL, NULL);
    eprint(ret);

    kernel1 = clCreateKernel(program1, "OddEven", &ret);
    eprint(ret);
    kernel2 = clCreateKernel(program2, "EvenOdd", &ret);
    eprint(ret);
    ret = clSetKernelArg(kernel1, 0, sizeof(cl_mem), (void *) &listObj);
    eprint(ret);
    ret = clSetKernelArg(kernel2, 0, sizeof(cl_mem), (void *) &listObj);
    eprint(ret);

    size_t global_item_size = LS - 1;
    size_t local_item_size = 1;
    
    for ( int i = 0; i <= LS / 2; i++ ) {
        ret = clEnqueueNDRangeKernel(cmdq, kernel1, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
        eprint(ret);
        ret = clEnqueueNDRangeKernel(cmdq, kernel2, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
        eprint(ret);
    }
    
    ret = clFinish(cmdq);
    eprint(ret);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    eprint(ret);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    eprint(ret);
    tot_time = (double)(time_end - time_start);

    ret = clEnqueueReadBuffer(cmdq, listObj, CL_TRUE, 0, LS * sizeof(int), list, 0, NULL, NULL);
    eprint(ret);

    //display result
    printf("SORT >> ");
    for ( int i = 0; i < LS; i++ ) {
        printf("%d ", list[i]);
    }

    //cleanup
    ret = clFlush(cmdq);
    ret = clReleaseKernel(kernel1);
    ret = clReleaseKernel(kernel2);
    ret = clReleaseProgram(program1);
    ret = clReleaseProgram(program2);
    ret = clReleaseMemObject(listObj);
    ret = clReleaseCommandQueue(cmdq);
    ret = clReleaseContext(context);
    free(list);


    end = clock();
    printf("\n============================= TIME INFO =============================\n");
    printf("Time for kernel execution >> %20.20fs\n", tot_time / 1000000);
    printf("Time for driver execution >> %20.20fs\n", (end - start) / (double)CLOCKS_PER_SEC);
    return 0;
}
