#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>


const int ARRAY_SIZE = 1000000;
/*
1. 创建平台
2. 创建设备
3. 根据设备创建上下文
*/

cl_context CreateContext(cl_device_id* device) {
	cl_int errNum;
	cl_uint numPlatforms;
	cl_platform_id firstPlatformId;
	cl_context context = NULL;
	errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);

	if (errNum != CL_SUCCESS || numPlatforms <= 0) {
		printf("Failed to find any OpenCL platforms.\n");
		return NULL;
	}
	errNum = clGetDeviceIDs(firstPlatformId, CL_DEVICE_TYPE_GPU, 1, device, NULL);
	if (errNum != CL_SUCCESS) {
		printf("There is no GPU , trying CPU...\n");
		errNum = clGetDeviceIDs(firstPlatformId, CL_DEVICE_TYPE_CPU, 1, device, NULL);
	}
	if (errNum != CL_SUCCESS) {
		printf("There is NO CPU or GPU\n");
		return NULL;
	}
	context = clCreateContext(NULL, 1, device, NULL, NULL, &errNum);
	if (errNum != CL_SUCCESS) {
		printf("Create context error\n");
		return NULL;
	}
	return context;
}
/*
 * 在上下文可用的第一个设备中创建命令队列
**/

cl_command_queue CreateCommandQueue(cl_context context, cl_device_id device) {
	//cl_int errNum;
	cl_command_queue commandQueue = NULL;
	//commandQueue = clCreateCommandQueue(context, device, 0, NULL);
	// OpenCL 2.0 的用法
	commandQueue = clCreateCommandQueueWithProperties(context, device, 0, NULL);
	if (commandQueue == NULL) {
		printf("Failed to create commandQueue for device 0\n");
		return NULL;
	}
	return commandQueue;
}

char* ReadKernelSourceFile(const char* filename, size_t* length) {
	FILE* file = NULL;
	size_t sourceLenth;
	char* sourceString;
	size_t ret;
	file = fopen(filename, "rb");
	if (file == NULL) {
		printf("%s at %d : Can't open %s \n", __FILE__, __LINE__ - 2, filename);
		return NULL;
	}
	//重定位到文件末尾
	fseek(file, 0, SEEK_END);
	sourceLenth = ftell(file);
	//重定位到文件开头
	fseek(file, 0, SEEK_SET);
	sourceString = (char*)malloc(sourceLenth + 1);
	if (sourceString == NULL) {
		return NULL;
	}
	sourceString[0] = '\0';
	ret = fread(sourceString, sourceLenth, 1, file);
	if (ret == 0) {
		printf("%s at %d : Cant't read source %s\n", __FILE__, __LINE__ - 2, filename);
		return NULL;
	}
	fclose(file);
	if (length != 0) {
		*length = sourceLenth;
	}
	sourceString[sourceLenth] = '\0';
	return sourceString;
}

cl_program CreateProgram(cl_context context, cl_device_id device, const char* filename) {
	cl_int errNum;
	cl_program program;
	//记录大小的数据类型
	size_t program_length;
	char* const source = ReadKernelSourceFile(filename, &program_length);
	program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, NULL);

	if (program == NULL) {
		printf("Failed to creae CL program from source.\n");
		return NULL;
	}
	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (errNum != CL_SUCCESS) {
		//char buildLog[16384];
		unsigned int size = 16384;
		char* buildLog = (char*)malloc(size);
		if (buildLog != NULL) {
			clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, size, buildLog, NULL);
			printf("Error in kernel : %s \n", buildLog);
			clReleaseProgram(program);
		}
		free(buildLog); buildLog = NULL;
		return NULL;
	}
	return program;
}
/*
@ 创建内存对象
*/
bool CreateMemObjects(cl_context context, cl_mem memObjects[3], float* a, float* b) {
	memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * ARRAY_SIZE, a, NULL);
	memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * ARRAY_SIZE, b, NULL);
	memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * ARRAY_SIZE, NULL, NULL);
	if (memObjects[0] == NULL || memObjects[1] == NULL || memObjects[2] == NULL) {
		printf("Error creating memeory objects.\n");
		return false;

	}
	return true;
}
/*
@ 清理OpenCL资源
*/
void CleanUp(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel kernel, cl_mem memObjects[3]) {
	for (int i = 0; i < 3; i++) {
		if (memObjects[i] != 0) {
			clReleaseMemObject(memObjects[i]);
		}
	}

	if (commandQueue != 0) {
		clReleaseCommandQueue(commandQueue);
	}
	if (kernel != 0) {
		clReleaseKernel(kernel);
	}
	if (program != 0) {
		clReleaseProgram(program);
	}
	if (context != 0) {
		clReleaseContext(context);
	}
}

/*main函数*/
int main_calc(int argc, char** agrv) {
	cl_context context = 0;
	cl_command_queue commandQueue = 0;
	cl_program program = 0;
	cl_device_id device = 0;
	cl_kernel kernel = 0;
	cl_mem memObjects[3] = { 0,0,0 };
	cl_int errNum;
	//创建OpenCL上下文
	context = CreateContext(&device);
	if (context == NULL) {
		printf("Failed to create OpenCL context\n");
		return 1;
	}
	//获得OpenCL设备，并创建命令队列
	commandQueue = CreateCommandQueue(context, device);
	if (commandQueue == NULL) {
		CleanUp(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	//创建OpenCL程序
	program = CreateProgram(context, device, "vecAdd.cl");
	if (program == NULL) {
		CleanUp(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	//创建内核对象
	kernel = clCreateKernel(program, "vector_add", NULL);
	if (kernel == NULL) {
		printf("Failed to create kernel\n");
		CleanUp(context, commandQueue, program, kernel, memObjects);
		return 1;

	}
	//创建OpenCL内存对象
	float *result = (float*)malloc(ARRAY_SIZE * sizeof(float));
	float *a = (float*)malloc(ARRAY_SIZE * sizeof(float));
	float *b = (float*)malloc(ARRAY_SIZE * sizeof(float));
	if (result == NULL || a == NULL || b == NULL) {
		return 1;
	}
	for (int i = 0; i < ARRAY_SIZE; i++) {
		a[i] = (float)i;
		b[i] = (float)(i * 2);
	}
	if (!CreateMemObjects(context, memObjects, a, b)) {
		CleanUp(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	//设置内核参数
	errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
	errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);
	errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[2]);

	if (errNum != CL_SUCCESS) {
		printf("Error setting kernel arguments.\n");
		CleanUp(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	size_t globalWorkSize[1] = { ARRAY_SIZE };
	size_t localWorkSize[1] = { 1 };
	//执行内核
	errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
	if (errNum != CL_SUCCESS) {
		printf("Error queueing kernel for execution\n");
		CleanUp(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	//将计算的结果拷贝到主机上
	errNum = clEnqueueReadBuffer(commandQueue, memObjects[2], CL_TRUE, 0, ARRAY_SIZE * sizeof(float), result, 0, NULL, NULL);
	if (errNum != CL_SUCCESS) {
		printf("Error reading result buffer.\n");
		CleanUp(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
	for (int i = 0; i < ARRAY_SIZE; i++) {
		printf("i=%d:%f\n", i, result[i]);
	}
	printf("Executed program succesfully\n");
	CleanUp(context, commandQueue, program, kernel, memObjects);

	free(result); result = NULL;
	free(a); a = NULL;
	free(b); b = NULL;
	return 0;
}

int main_platform(int argc, char** argv)
{
	cl_platform_id* platform;
	cl_uint num_platform;
	cl_int err;
	err = clGetPlatformIDs(0, NULL, &num_platform);
	platform = (cl_platform_id*)malloc(sizeof(cl_platform_id) * num_platform);
	err = clGetPlatformIDs(num_platform, platform, NULL);
	for (unsigned int i = 0; i < num_platform; i++) {
		printf("\nPlatform %d information\n", i);
		size_t size;
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, 0, NULL, &size);
		char* PName = (char*)malloc(size);
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, size, PName, NULL);
		printf("CL_PLATFORM_NAME: %s\n", PName);
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_VENDOR, 0, NULL, &size);
		char* PVendor = (char*)malloc(size);
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_VENDOR, size, PVendor, NULL);
		printf("CL_PLATFORM_VENDOR: %s\n", PVendor);
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_VERSION, 0, NULL, &size);
		char* PVersion = (char*)malloc(size);
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_VERSION, size, PVersion, NULL);
		printf("CL_PLATFORM_VERSION: %s\n", PVersion);
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_PROFILE, 0, NULL, &size);
		char* PProfile = (char*)malloc(size);
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_PROFILE, size, PProfile, NULL);
		printf("CL_PLATFORM_PROFILE: %s\n", PProfile);
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_EXTENSIONS, 0, NULL, &size);
		char* PExten = (char*)malloc(size);
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_EXTENSIONS, size, PExten, NULL);
		printf("CL_PLATFORM_EXTENSIONS: %s\n", PExten);
		free(PName);
		free(PVendor);
		free(PVersion);
		free(PProfile);
		free(PExten);
	}
	return 0;
}

int main(int argc, char** argv)
{
	main_calc(argc, argv);

	main_platform(argc, argv);

	return 0;
}