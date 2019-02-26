/* 
    @Author:    3sne ( Mukur Panchani )
    @FileName:  q1CharSelectionSort.c
    @Task:      OpenCL program to sort a string character wise using Selection Sort.
 */

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <string.h>
#define MAX_SOURCE_SIZE (0x100000)

int main() {
	int i;
	int LS, len;
	char *buf = (char *)malloc(sizeof(int) * 102400);
	printf("Enter a String >> ");
	scanf("%[^\n]s", buf);
	LS = strlen(buf);
	char *A = (char *)malloc(sizeof(char) * LS);
	strcpy(A, buf);
	printf("Input is  >> %s\n", A);
	char *B = (char *)malloc(sizeof(char) * LS);

	FILE *fp;
	char *source_str;
	size_t source_size;
	fp = fopen("q1.cl", "r");
	if(!fp) {
		fprintf(stderr, "Failed to load Kernel\n");
		getchar();
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	//Create Context
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	//Create Command Queue
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

	//Create memory buffers in device
	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, LS * sizeof(char),NULL, &ret);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, LS * sizeof(char),NULL, &ret);
	ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, LS * sizeof(char), A, 0, NULL, NULL);

	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	ret = clBuildProgram(program,1,&device_id,NULL, NULL, NULL);

	cl_kernel kernel = clCreateKernel(program, "selSort", &ret);
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);

	size_t global_item_size = LS;
	size_t local_item_size = 1;
	cl_event event;
	ret = clEnqueueNDRangeKernel(command_queue,kernel,1,NULL,&global_item_size,&local_item_size,0,NULL,NULL);
	ret = clFinish(command_queue);


	ret =clEnqueueReadBuffer(command_queue, b_mem_obj, CL_TRUE, 0, LS * sizeof(char), B, 0, NULL, NULL);

	//result	
	printf("Output is >> %s\n", B);

	ret = clFlush(command_queue);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a_mem_obj);
	ret = clReleaseMemObject(b_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(A);
	free(B);
	free(buf);
	getchar();
	return 0;
}