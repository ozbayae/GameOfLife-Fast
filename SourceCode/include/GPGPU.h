#pragma once
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "CL/cl.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

#define SUCCESS 0
#define FAILURE 1
class GPGPU
{

public:

	struct CLinfo {
		cl_platform_id platform;
		cl_device_id* devices;
		cl_context context;
		cl_command_queue commandQueue;
		cl_program program;
	} clinfo;

		GPGPU(const char* kernels_path);
		virtual ~GPGPU();

private:
	int convertToString(const char* filename, std::string& s);
};

class Kernel : GPGPU 
{
public:
	
	cl_kernel kernel;
	Kernel(const char* kernels_path, const char* kernel_name);
	~Kernel();
};