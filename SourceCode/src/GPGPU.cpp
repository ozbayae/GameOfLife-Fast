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


