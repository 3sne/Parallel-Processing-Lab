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
    
    char *buf = (char *)malloc(sizeof(int) * 102400);

  //user input
    int inChoice, len, temp;
    printf("Wanna generate random string(0) or give own input(1)? ");
    scanf("%d", &inChoice);
    if (inChoice == 0) {
        len = rand() % 999 + 1;
        for ( int i = 0; i < len; i++ ) {
            temp = rand() % 110 + 15;
            while ( (temp < 65 && temp > 90) && (temp < 97 && temp > 122)) {
                temp = rand() % 110 + 15;
            }
            buf[i] = temp;
        }
        printf("Input generated >> %s\n", buf);
    } else if ( inChoice == 1 ) {
        printf("Enter a String  >> ");
	    scanf("%s", buf);
        len = strlen(buf);
    } else {
        printf("\nBad Option, exiting...\n");
        exit(EXIT_FAILURE);
    }

    char *inStr = (char*)malloc(sizeof(char) * len);
    strcpy(inStr, buf);
    

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
    eprint("plat",ret);
    ret = clGetDeviceIDs(platid, CL_DEVICE_TYPE_ALL, 1, &deviceid, &numDevices);                                //2
    eprint("dev", ret);
    context = clCreateContext(NULL, 1, &deviceid, NULL, NULL, &ret);                                            //3
    cmdq = clCreateCommandQueue(context, deviceid, CL_QUEUE_PROFILING_ENABLE, &ret);                            //4
    cl_mem strObj = clCreateBuffer(context, CL_MEM_READ_WRITE, len * sizeof(char), NULL, &ret);                //5
    eprint("strobj", ret);
    ret = clEnqueueWriteBuffer(cmdq, strObj, CL_TRUE, 0, len * sizeof(char), inStr, 0, NULL, NULL);         //6
    eprint("strobj Write", ret);

    program = clCreateProgramWithSource(context, 1, (const char**) &srcStr, (const size_t*)&SS, &ret);                       //7
    eprint("prog src", ret);
    ret = clBuildProgram(program, 1, &deviceid, NULL, NULL, NULL);
    eprint("build prog", ret);
    kernel = clCreateKernel(program, "reverseIt", &ret);                                                         //8
    eprint("create kernel", ret);
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &strObj);                                      //9
    eprint("kernel arg 1", ret);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_int), &len);
    eprint("kernel arg 2", ret);

    size_t global_item_size = len / 2;                                                                               //10
    size_t local_item_size = 1;                                                                                //10
    ret = clEnqueueNDRangeKernel(cmdq, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);  //11
    ret = clFinish(cmdq);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    tot_time = (double)(time_end - time_start);
    ret = clEnqueueReadBuffer(cmdq, strObj, CL_TRUE, 0, len * sizeof(char), inStr, 0, NULL, NULL);          //12
    eprint("read", ret);
    
    //display result
    printf("Reverse is      >> %s\n", inStr);

    //cleanup
    ret = clFlush(cmdq);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(strObj);
    ret = clReleaseCommandQueue(cmdq);
    ret = clReleaseContext(context);
    free(inStr);
    free(buf);

    end = clock();
    printf("=============================CONCLUSION=============================\n");
    printf("Time for kernel execution >> %20.20fs\n", tot_time / 1000000);
    printf("Time for driver execution >> %20.20fs\n", (end - start) / (double)CLOCKS_PER_SEC);
    return 0;
}
