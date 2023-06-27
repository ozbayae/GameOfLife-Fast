#include "Life.h"
#include "Stopwatch.h"
//const char* getErrorString(cl_int error);


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
	for (unsigned int j = 1; j <= width; j++)
	{
		for (unsigned int i = 1; i <= height; i++) {
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
	////status = clEnqueueWriteBuffer(clinfo.commandQueue, clinfo.gridBuffer, CL_TRUE, 0, sizeof(short) * grid_width * (height + 2), grid, 0, NULL, NULL);
	//if (!swapped) {
	//	status = clEnqueueNDRangeKernel(clinfo.commandQueue, clinfo.kernelUpdate, 2, NULL, clinfo.globalWorkSize, NULL, 0, NULL, NULL);
	//	status = clEnqueueReadBuffer(clinfo.commandQueue, clinfo.new_gridBuffer, CL_TRUE, 0, sizeof(bool) * grid_width * grid_height, new_grid, 0, NULL, NULL);
	//}
	//else {
	//	status = clEnqueueNDRangeKernel(clinfo.commandQueue, clinfo.kernelUpdateSwapped, 2, NULL, clinfo.globalWorkSize, NULL, 0, NULL, NULL);
	//	status = clEnqueueReadBuffer(clinfo.commandQueue, clinfo.gridBuffer, CL_TRUE, 0, sizeof(bool) * grid_width * grid_height, new_grid, 0, NULL, NULL);
	//}
	//swapGrids();
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

	clinfo.gridBuffer = clCreateBuffer(clinfo.context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, grid_width * grid_height * sizeof(bool), (void*)grid, NULL);
	clinfo.new_gridBuffer = clCreateBuffer(clinfo.context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, grid_width * grid_height * sizeof(bool), (void*)new_grid, NULL);

	/*Step 8: Create kernel object */
	clinfo.kernelUpdate = clCreateKernel(clinfo.program, "update", NULL);
	clinfo.kernelUpdateSwapped = clCreateKernel(clinfo.program, "update", NULL);

	//cl_int width = this->width;
	/*Step 9: Sets Kernel arguments.*/
	status = clSetKernelArg(clinfo.kernelUpdate, 0, sizeof(cl_mem), &(clinfo.gridBuffer));
	status = clSetKernelArg(clinfo.kernelUpdate, 1, sizeof(cl_mem), &(clinfo.new_gridBuffer));
	status = clSetKernelArg(clinfo.kernelUpdate, 2, sizeof(cl_int), &grid_width);

	status = clSetKernelArg(clinfo.kernelUpdateSwapped, 0, sizeof(cl_mem), &(clinfo.new_gridBuffer)); //swapped
	status = clSetKernelArg(clinfo.kernelUpdateSwapped, 1, sizeof(cl_mem), &(clinfo.gridBuffer)); //swapped
	status = clSetKernelArg(clinfo.kernelUpdateSwapped, 2, sizeof(cl_int), &grid_width);

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

void Life::randomize() {
	for (int i = 0; i < .6 * width * height; i++)
	{
		int x = rand() % width + 1;
		int y = rand() % height + 1;
		setLife(x, y, 1);
	}
	clEnqueueWriteBuffer(clinfo.commandQueue, clinfo.gridBuffer, CL_TRUE, 0, sizeof(bool) * grid_width * grid_height, grid, 0, NULL, NULL);
}

cl_mem Life::getGridBuffer()
{
	return clinfo.gridBuffer;
}
