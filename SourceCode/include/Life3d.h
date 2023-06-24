#include "World3d.h"
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

using namespace world3;

#ifndef LIFE3D_H
#define LIFE3D_H


namespace life3
{
	class Life3d : public World3d
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
			size_t globalWorkSize[3];
		} clinfo;

		int convertToString(const char* filename, std::string& s);

	public:
		Life3d(int x, int y, int z) : World3d(x, y, z){
			setupCL();
		}

		~Life3d();

		virtual void update();
		int setupCL();
		void randomize();
	};
}
#endif
