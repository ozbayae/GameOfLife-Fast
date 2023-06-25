#include "GPGPU.h"

GPGPU::GPGPU(const char* kernels_path)
{
	/*Step1: Getting platforms and choose an available one.*/
	cl_uint numPlatforms;	//the NO. of platforms
	clinfo.platform = NULL;	//the chosen platform
	cl_int	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
	{
		std::cout << "Error: Getting platforms!" << std::endl;
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
		std::cout << "No GPU device available." << std::endl;
		std::cout << "Choose CPU as default device." << std::endl;
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
	std::string sourceStr;
	status = convertToString(kernels_path, sourceStr);
	const char* source = sourceStr.c_str();
	size_t sourceSize[] = { strlen(source) };
	clinfo.program = clCreateProgramWithSource(clinfo.context, 1, &source, sourceSize, NULL);
	/*Step 6: Build program. */
	status = clBuildProgram(clinfo.program, 1, devices, NULL, NULL, NULL);
}

//destructor for GPGPU class
GPGPU::~GPGPU()
{
	cl_int status;

	status = clReleaseProgram(clinfo.program);				//Release the program object.
	status = clReleaseCommandQueue(clinfo.commandQueue);	//Release  Command queue.
	status = clReleaseContext(clinfo.context);				//Release context.

	if (clinfo.devices != NULL)
	{
		free(clinfo.devices);
		clinfo.devices = NULL;
	}
}

Kernel::Kernel(const char* kernels_path, const char* kernel_name) : GPGPU(kernels_path)
{
	kernel = clCreateKernel(clinfo.program, kernel_name, NULL);
}

//destructor for Kernel class
Kernel::~Kernel()
{
	cl_int status;
	status = clReleaseKernel(kernel);	//Release kernel.
}

/* convert the kernel file into a string */
int GPGPU::convertToString(const char* filename, std::string& s)
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
	std::cout << "Error: failed to open file\n:" << filename << std::endl;
	return FAILURE;
}

cl_platform_id Kernel::getPlatform()
{
	return clinfo.platform;
}

cl_device_id* Kernel::getDevices()
{
	return clinfo.devices;
}

cl_context Kernel::getContext()
{
	return clinfo.context;
}

cl_command_queue Kernel::getCommandQueue()
{
	return clinfo.commandQueue;
}

cl_program Kernel::getProgram()
{
	return clinfo.program;
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


