#include "Life.h"
#include "Stopwatch.h"
const char* getErrorString(cl_int error);


Life::~Life()
{
	cl_int status;
	/*Step 12: Clean the resources.*/
	status = clReleaseKernel(clinfo.kernelUpdate);			//Release kernel.
	status = clReleaseKernel(clinfo.kernelUpdateSwapped);	//Release kernel.

	status = clReleaseProgram(clinfo.program);				//Release the program object.
	status = clReleaseMemObject(clinfo.gridBuffer);		//Release mem object.
	status = clReleaseMemObject(clinfo.new_gridBuffer);
	status = clReleaseCommandQueue(clinfo.commandQueue);	//Release  Command queue.
	status = clReleaseContext(clinfo.context);				//Release context.

	if (clinfo.devices != NULL)
	{
		free(clinfo.devices);
		clinfo.devices = NULL;
	}
}

void Life::update()
{
	Stopwatch tm;

	tm.start();
	for (unsigned int i = 1; i <= height; i++)
	{
		for (unsigned int j = 1; j <= width; j++)
		{
			int m = getNeighbors(j, i, 1);
			if (m == 3)
			{
				setNewLife(j, i, 1);
			}
			if (m == 2)
			{
				setNewLife(j, i, getLifeform(j, i));
			}
			if (m != 3 && m != 2)
			{
				setNewLife(j, i, 0);
			}
		}
	}
	swapGrids();


	//cl_int status;
	////write data to device
	////status = clEnqueueWriteBuffer(clinfo.commandQueue, clinfo.gridBuffer, CL_TRUE, 0, sizeof(short) * (width + 2) * (height + 2), grid, 0, NULL, NULL);
	//if (!swapped) {
	//	status = clEnqueueNDRangeKernel(clinfo.commandQueue, clinfo.kernelUpdate, 2, NULL, clinfo.globalWorkSize, NULL, 0, NULL, NULL);
	//	status = clEnqueueReadBuffer(clinfo.commandQueue, clinfo.new_gridBuffer, CL_TRUE, 0, sizeof(short) * (width + 2) * (height + 2), new_grid, 0, NULL, NULL);
	//}
	//else {
	//	status = clEnqueueNDRangeKernel(clinfo.commandQueue, clinfo.kernelUpdateSwapped, 2, NULL, clinfo.globalWorkSize, NULL, 0, NULL, NULL);
	//	status = clEnqueueReadBuffer(clinfo.commandQueue, clinfo.gridBuffer, CL_TRUE, 0, sizeof(short) * (width + 2) * (height + 2), grid, 0, NULL, NULL);
	//}
	//swapGrids();

	tm.stop("Simulation 2D");
}

int Life::setupCL() {
	/*Step1: Getting platforms and choose an available one.*/
	cl_uint numPlatforms;	//the NO. of platforms
	clinfo.platform = NULL;	//the chosen platform
	cl_int	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
	{
		cout << "Error: Getting platforms!" << endl;
		return FAILURE;
	}

	/*For clarity, choose the first available platform. */
	if (numPlatforms > 0)
	{
		cl_platform_id* platforms = (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, NULL);
		clinfo.platform = platforms[0];
		free(platforms);
	}

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	cl_uint				numDevices = 0;
	cl_device_id* devices;
	status = clGetDeviceIDs(clinfo.platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
	if (numDevices == 0)	//no GPU available.
	{
		cout << "No GPU device available." << endl;
		cout << "Choose CPU as default device." << endl;
		status = clGetDeviceIDs(clinfo.platform, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);
		devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(clinfo.platform, CL_DEVICE_TYPE_CPU, numDevices, devices, NULL);
	}
	else
	{
		devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(clinfo.platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
	}
	clinfo.devices = devices;


	/*Step 3: Create context.*/
	clinfo.context = clCreateContext(NULL, 1, clinfo.devices, NULL, NULL, NULL);

	/*Step 4: Creating command queue associate with the context.*/
	clinfo.commandQueue = clCreateCommandQueue(clinfo.context, clinfo.devices[0], 0, NULL);

	/*Step 5: Create program object */
	const char* filename = "src/kernels.cl";
	std::string sourceStr;
	status = convertToString(filename, sourceStr);
	const char* source = sourceStr.c_str();
	size_t sourceSize[] = { strlen(source) };
	clinfo.program = clCreateProgramWithSource(clinfo.context, 1, &source, sourceSize, NULL);
	/*Step 6: Build program. */
	status = clBuildProgram(clinfo.program, 1, devices, NULL, NULL, NULL);

	clinfo.gridBuffer = clCreateBuffer(clinfo.context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, (width + 2) * (height + 2) * sizeof(short), (void*)grid, NULL);
	clinfo.new_gridBuffer = clCreateBuffer(clinfo.context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, (width + 2) * (height + 2) * sizeof(short), (void*)new_grid, NULL);

	/*Step 8: Create kernel object */
	clinfo.kernelUpdate = clCreateKernel(clinfo.program, "update", NULL);
	clinfo.kernelUpdateSwapped = clCreateKernel(clinfo.program, "update", NULL);

	cl_int width = this->width;
	/*Step 9: Sets Kernel arguments.*/
	status = clSetKernelArg(clinfo.kernelUpdate, 0, sizeof(cl_mem), &(clinfo.gridBuffer));
	status = clSetKernelArg(clinfo.kernelUpdate, 1, sizeof(cl_mem), &(clinfo.new_gridBuffer));
	status = clSetKernelArg(clinfo.kernelUpdate, 2, sizeof(cl_int), &width);

	status = clSetKernelArg(clinfo.kernelUpdateSwapped, 0, sizeof(cl_mem), &(clinfo.new_gridBuffer)); //swapped
	status = clSetKernelArg(clinfo.kernelUpdateSwapped, 1, sizeof(cl_mem), &(clinfo.gridBuffer)); //swapped
	status = clSetKernelArg(clinfo.kernelUpdateSwapped, 2, sizeof(cl_int), &width);

	//set global work size
	clinfo.globalWorkSize[0] = width;
	clinfo.globalWorkSize[1] = height;
}

/* convert the kernel file into a string */
int Life::convertToString(const char* filename, std::string& s)
{
	size_t size;
	char* str;
	std::fstream f(filename, (std::fstream::in | std::fstream::binary));

	if (f.is_open())
	{
		size_t fileSize;
		f.seekg(0, std::fstream::end);
		size = fileSize = (size_t)f.tellg();
		f.seekg(0, std::fstream::beg);
		str = new char[size + 1];
		if (!str)
		{
			f.close();
			return 0;
		}

		f.read(str, fileSize);
		f.close();
		str[size] = '\0';
		s = str;
		delete[] str;
		return 0;
	}
	cout << "Error: failed to open file\n:" << filename << endl;
	return FAILURE;
}
const char* getErrorString(cl_int error)
{
	switch (error) {
		// run-time and JIT compiler errors
	case 0: return "CL_SUCCESS";
	case -1: return "CL_DEVICE_NOT_FOUND";
	case -2: return "CL_DEVICE_NOT_AVAILABLE";
	case -3: return "CL_COMPILER_NOT_AVAILABLE";
	case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5: return "CL_OUT_OF_RESOURCES";
	case -6: return "CL_OUT_OF_HOST_MEMORY";
	case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8: return "CL_MEM_COPY_OVERLAP";
	case -9: return "CL_IMAGE_FORMAT_MISMATCH";
	case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11: return "CL_BUILD_PROGRAM_FAILURE";
	case -12: return "CL_MAP_FAILURE";
	case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case -15: return "CL_COMPILE_PROGRAM_FAILURE";
	case -16: return "CL_LINKER_NOT_AVAILABLE";
	case -17: return "CL_LINK_PROGRAM_FAILURE";
	case -18: return "CL_DEVICE_PARTITION_FAILED";
	case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

		// compile-time errors
	case -30: return "CL_INVALID_VALUE";
	case -31: return "CL_INVALID_DEVICE_TYPE";
	case -32: return "CL_INVALID_PLATFORM";
	case -33: return "CL_INVALID_DEVICE";
	case -34: return "CL_INVALID_CONTEXT";
	case -35: return "CL_INVALID_QUEUE_PROPERTIES";
	case -36: return "CL_INVALID_COMMAND_QUEUE";
	case -37: return "CL_INVALID_HOST_PTR";
	case -38: return "CL_INVALID_MEM_OBJECT";
	case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40: return "CL_INVALID_IMAGE_SIZE";
	case -41: return "CL_INVALID_SAMPLER";
	case -42: return "CL_INVALID_BINARY";
	case -43: return "CL_INVALID_BUILD_OPTIONS";
	case -44: return "CL_INVALID_PROGRAM";
	case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46: return "CL_INVALID_KERNEL_NAME";
	case -47: return "CL_INVALID_KERNEL_DEFINITION";
	case -48: return "CL_INVALID_KERNEL";
	case -49: return "CL_INVALID_ARG_INDEX";
	case -50: return "CL_INVALID_ARG_VALUE";
	case -51: return "CL_INVALID_ARG_SIZE";
	case -52: return "CL_INVALID_KERNEL_ARGS";
	case -53: return "CL_INVALID_WORK_DIMENSION";
	case -54: return "CL_INVALID_WORK_GROUP_SIZE";
	case -55: return "CL_INVALID_WORK_ITEM_SIZE";
	case -56: return "CL_INVALID_GLOBAL_OFFSET";
	case -57: return "CL_INVALID_EVENT_WAIT_LIST";
	case -58: return "CL_INVALID_EVENT";
	case -59: return "CL_INVALID_OPERATION";
	case -60: return "CL_INVALID_GL_OBJECT";
	case -61: return "CL_INVALID_BUFFER_SIZE";
	case -62: return "CL_INVALID_MIP_LEVEL";
	case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
	case -64: return "CL_INVALID_PROPERTY";
	case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
	case -66: return "CL_INVALID_COMPILER_OPTIONS";
	case -67: return "CL_INVALID_LINKER_OPTIONS";
	case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

		// extension errors
	case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
	case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
	case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
	case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
	case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
	case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
	default: return "Unknown OpenCL error";
	}
}

void Life::randomize() {
	for (int i = 0; i < .6 * width * height; i++)
	{
		int x = rand() % width + 1;
		int y = rand() % height + 1;
		setLife(x, y, 1);
	}
	clEnqueueWriteBuffer(clinfo.commandQueue, clinfo.gridBuffer, CL_TRUE, 0, sizeof(short) * (width + 2) * (height + 2), grid, 0, NULL, NULL);
}