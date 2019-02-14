/* 
    Converts binary numbers to their respective one's compliment.
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
    int *binList = (int*)malloc(sizeof(int) * LS);
    int *onesList = (int*)malloc(sizeof(int) * LS);
    for ( int i = 0; i < LS; i++ ) {
        binList[i] = 0;
        int limit = (rand() % 8) + 2;
        for ( int j = 0; j < limit; j++ ) {
            binList[i] = (binList[i] * 10) + (rand() % 2);
        }
    }

  //kernel src load
    FILE *f;
    char *srcStr;
    size_t SS;
    f = fopen("q2.cl", "r");
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
    eprint(ret);
    ret = clGetDeviceIDs(platid, CL_DEVICE_TYPE_ALL, 1, &deviceid, &numDevices);                                
    eprint(ret);
    context = clCreateContext(NULL, 1, &deviceid, NULL, NULL, &ret);                                            
    eprint(ret);
    cmdq = clCreateCommandQueue(context, deviceid, CL_QUEUE_PROFILING_ENABLE, &ret);                            
    eprint(ret);
    cl_mem binObj = clCreateBuffer(context, CL_MEM_READ_ONLY, LS * sizeof(int), NULL, &ret);                
    eprint(ret);
    cl_mem onesObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, LS * sizeof(int), NULL, &ret);               
    eprint(ret);
    ret = clEnqueueWriteBuffer(cmdq, binObj, CL_TRUE, 0, LS * sizeof(int), binList, 0, NULL, NULL);         
    eprint(ret);
    program = clCreateProgramWithSource(context, 1, (const char**) &srcStr, (const size_t*)&SS, &ret);                       
    eprint(ret);
    ret = clBuildProgram(program, 1, &deviceid, NULL, NULL, NULL);
    eprint(ret);
    kernel = clCreateKernel(program, "bto", &ret);                                                         
    eprint(ret);
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &binObj);                                      
    eprint(ret);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &onesObj);
    eprint(ret);
    size_t global_item_size = LS;                                                                               
    size_t local_item_size = 1;                                                                                
    ret = clEnqueueNDRangeKernel(cmdq, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);  
    eprint(ret);
    ret = clFinish(cmdq);
    eprint(ret);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    eprint(ret);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    eprint(ret);
    tot_time = (double)(time_end - time_start);
    ret = clEnqueueReadBuffer(cmdq, onesObj, CL_TRUE, 0, LS * sizeof(int), onesList, 0, NULL, NULL);          
    eprint(ret);
    
    //display result
    for ( int i = 0; i < LS; i++ ) {
        printf("%d -> %d\n", binList[i], onesList[i]);
    }

    //cleanup
    ret = clFlush(cmdq);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(binObj);
    ret = clReleaseMemObject(onesObj);
    ret = clReleaseCommandQueue(cmdq);
    ret = clReleaseContext(context);
    free(binList);
    free(onesList); 
    

    end = clock();
    printf("=============================CONCLUSION=============================\n");
    printf("Time for kernel execution >> %20.20fs\n", tot_time / 1000000);
    printf("Time for driver execution >> %20.20fs\n", (end - start) / (double)CLOCKS_PER_SEC);
    return 0;
}
