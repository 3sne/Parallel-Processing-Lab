/* 
    Swaps alternate elements of an array parallely using OpenCL
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
    const int LS = 102400;
    int *arr = (int*)malloc(sizeof(int) * LS);
    for ( int i = 0; i < LS; i++ ) {
        arr[i] = i;
    }
    printf("Befor >> ");
    for ( int i = 0; i < LS; i++ ) {
        printf("%d ", arr[i]);
    }
    printf("\n");


  //kernel src load
    FILE *f;
    char *srcStr;
    size_t SS;
    f = fopen("q3.cl", "r");
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
    ret = clGetPlatformIDs(1, &platid, &numPlats);                                                              //1
    eprint(ret);
    ret = clGetDeviceIDs(platid, CL_DEVICE_TYPE_ALL, 1, &deviceid, &numDevices);                                //2
    eprint(ret);
    context = clCreateContext(NULL, 1, &deviceid, NULL, NULL, &ret);                                            //3
    eprint(ret);
    cmdq = clCreateCommandQueue(context, deviceid, CL_QUEUE_PROFILING_ENABLE, &ret);                            //4
    eprint(ret);
    cl_mem arr_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, LS * sizeof(int), NULL, &ret);                //5
    eprint(ret);
    ret = clEnqueueWriteBuffer(cmdq, arr_mem_obj, CL_TRUE, 0, LS * sizeof(int), arr, 0, NULL, NULL);         //6
    eprint(ret);
    program = clCreateProgramWithSource(context, 1, (const char**) &srcStr, (const size_t*)&SS, &ret);                       //7
    eprint(ret);
    ret = clBuildProgram(program, 1, &deviceid, NULL, NULL, NULL);
    eprint(ret);
    kernel = clCreateKernel(program, "swep", &ret);                                                         //8
    eprint(ret);
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &arr_mem_obj);                                      //9
    eprint(ret);
    size_t global_item_size = LS;                                                                               //10
    size_t local_item_size = 1;                                                                                //10
    ret = clEnqueueNDRangeKernel(cmdq, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);  //11
    eprint(ret);
    ret = clFinish(cmdq);
    eprint(ret);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    eprint(ret);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    eprint(ret);
    tot_time = (double)(time_end - time_start);
    ret = clEnqueueReadBuffer(cmdq, arr_mem_obj, CL_TRUE, 0, LS * sizeof(int), arr, 0, NULL, NULL);          //12
    eprint(ret);
    eprint(ret);


    //display result
    printf("After >> ");
    for ( int i = 0; i < LS; i++ ) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    
    //cleanup
    ret = clFlush(cmdq);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(arr_mem_obj);
    ret = clReleaseCommandQueue(cmdq);
    ret = clReleaseContext(context);
    free(arr);
    end = clock();
    
    
    printf("=============================CONCLUSION=============================\n");
    printf("Time for kernel execution >> %20.9fs\n", tot_time / 1000000);
    printf("Time for driver execution >> %20.9fs\n", (end - start) / (double)CLOCKS_PER_SEC);
    return 0;
}
