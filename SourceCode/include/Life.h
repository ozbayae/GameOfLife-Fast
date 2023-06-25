#pragma once
#include "World.h"
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "CL\cl.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

#define SUCCESS 0
#define FAILURE 1

class Life : public World
{
	struct CLinfo {
		cl_platform_id platform;
		cl_device_id* devices;
		cl_context context;
		cl_command_queue commandQueue;
		cl_program program;
		cl_kernel kernelUpdate;
		cl_kernel kernelUpdateSwapped;
		cl_mem gridBuffer;
		cl_mem new_gridBuffer;
		size_t globalWorkSize[2];
	} clinfo;

	int convertToString(const char* filename, std::string& s);

public:
	Life(int x, int y) : World(x, y)
	{
		std::cout << "Life constructor called" << std::endl;
		setupCL();
	}

	~Life();

	virtual void update();
	int setupCL();
	void randomize();
	//get gridbuffer
	cl_mem getGridBuffer();

};