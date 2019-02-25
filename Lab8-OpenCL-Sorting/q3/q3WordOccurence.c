/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q3WordOccurance
    @Task:      Find totals number of occurances in a string using OpenCL
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

void getWords(char *s, int *wordStart, int *wordEnd, int *wInd) {
    int f = 0, i = 0, bf =0;
    while (s[i] != '\0') {
        if ( s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
            if ( f == 1 ) {
                wordEnd[*wInd] = (i - 1);
                *wInd += 1;
            }
            f = 0;
        } else {
            if ( f == 0 ) {
                wordStart[*wInd] = i;
            }
            f = 1;
        }
        i++;
    }
    wordEnd[*wInd] = (i - 1);
}

int main(int argc, char const *argv[]) {
    clock_t start, end;
    start = clock();

  //user input
    int wInd = 0;
    int ourWordCount = 0;
    int *wordStart = (int*)malloc(sizeof(int) * 256);
    int *wordEnd = (int*)malloc(sizeof(int) * 256);
    char *inStr = (char*)malloc(sizeof(char) * 1024);
    char *ourWord = (char*)malloc(sizeof(char) * 1024);
    printf("Enter Da String >> ");
    scanf("%[^\n]s",inStr);
    int len = strlen(inStr);
    getWords(inStr, wordStart, wordEnd, &wInd);
    for (int i = 0; i <= wInd; i++ ) {
        printf("Hey >> %d: %d  |  %d\n", i, wordStart[i], wordEnd[i]);
    }
    printf("Enter word to find occurances for >> ");
    scanf("%s", ourWord);

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
    ret = clGetPlatformIDs(1, &platid, &numPlats);
    eprint("plat",ret);
    ret = clGetDeviceIDs(platid, CL_DEVICE_TYPE_ALL, 1, &deviceid, &numDevices);
    eprint("dev", ret);
    context = clCreateContext(NULL, 1, &deviceid, NULL, NULL, &ret);
    cmdq = clCreateCommandQueue(context, deviceid, CL_QUEUE_PROFILING_ENABLE, &ret);
    
    cl_mem inStrObj = clCreateBuffer(context, CL_MEM_READ_WRITE, 1024 * sizeof(char), NULL, &ret);
    eprint("inStrObj", ret);
    cl_mem w_start_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 256 * sizeof(int), NULL, &ret);
    eprint("w_start_obj", ret);
    cl_mem w_end_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 256 * sizeof(int), NULL, &ret);
    eprint("w_end_obj", ret);
    cl_mem ourWordObj = clCreateBuffer(context, CL_MEM_READ_WRITE, 1024 * sizeof(char), NULL, &ret);
    eprint("ourWordObj", ret);
    cl_mem ourWordCountObj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &ret);
    eprint("ourWordObj", ret);
    
    ret = clEnqueueWriteBuffer(cmdq, inStrObj, CL_TRUE, 0, 1024 * sizeof(char), inStr, 0, NULL, NULL);
    eprint("inStrObj Write", ret);
    ret = clEnqueueWriteBuffer(cmdq, w_start_obj, CL_TRUE, 0, 256 * sizeof(int), wordStart, 0, NULL, NULL);
    eprint("w_start_obj Write", ret);
    ret = clEnqueueWriteBuffer(cmdq, w_end_obj, CL_TRUE, 0, 256 * sizeof(int), wordEnd, 0, NULL, NULL);
    eprint("w_end_obj Write", ret);
    ret = clEnqueueWriteBuffer(cmdq, ourWordObj, CL_TRUE, 0, 1024 * sizeof(char), ourWord, 0, NULL, NULL);
    eprint("ourWordObj Write", ret);
    ret = clEnqueueWriteBuffer(cmdq, ourWordCountObj, CL_TRUE, 0, sizeof(int), ourWordCount, 0, NULL, NULL);
    eprint("ourWordCountObj Write", ret);

    program = clCreateProgramWithSource(context, 1, (const char**) &srcStr, (const size_t*)&SS, &ret);
    eprint("prog src", ret);
    ret = clBuildProgram(program, 1, &deviceid, NULL, NULL, NULL);
    eprint("build prog", ret);
    kernel = clCreateKernel(program, "reverseAll", &ret);
    eprint("create kernel", ret);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &inStrObj);
    eprint("kernel arg 0", ret);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &w_start_obj);
    eprint("kernel arg 1", ret);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *) &w_end_obj);
    eprint("kernel arg 2", ret);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *) &ourWordObj);
    eprint("kernel arg 3", ret);
    ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *) &ourWordCount);
    eprint("kernel arg 4", ret);

    size_t global_item_size = wInd + 1;
    size_t local_item_size = 1;
    ret = clEnqueueNDRangeKernel(cmdq, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
    ret = clFinish(cmdq);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    ret = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    tot_time = (double)(time_end - time_start);
    
    ret =clEnqueueReadBuffer(command_queue, ourWordCountObj, CL_TRUE, 0, sizeof(int), ourWordCount, 0, NULL, NULL);
    eprint(ret);

    //display result
    printf("Number of occurances are      >> %d\n", ourWordCount);

    //cleanup
    ret = clFlush(cmdq);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(inStrObj);
    ret = clReleaseMemObject(w_start_obj);
    ret = clReleaseMemObject(w_end_obj);
    ret = clReleaseMenObject(ourWordObj);
    ret = clReleaseMenObject(ourWordCountObj);
    ret = clReleaseCommandQueue(cmdq);
    ret = clReleaseContext(context);
    free(inStr);
    free(wordEnd);
    free(wordStart);
    free(ourWord);

    end = clock();
    printf("\n=============================TIME INFO=============================\n");
    printf("Time for kernel execution >> %20.20fs\n", tot_time / 1000000);
    printf("Time for driver execution >> %20.20fs\n", (end - start) / (double)CLOCKS_PER_SEC);
    return 0;
}
