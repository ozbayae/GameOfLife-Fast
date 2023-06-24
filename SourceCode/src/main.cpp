#include "World.h"
#include "Life.h"

#include <iostream>
#include "time.h"

//#include <stdlib.h>
//#include <Windows.h>

#include "GLScene.h"

#include<chrono>

using std::malloc;
using std::cout;
using std::endl;


//public:
//	Timer();
//	~Timer();
//
//	void Stop() {
//		auto endTimePoint = std::chrono::high_resolution_clock::now();
//
//		auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(startTimePoint).time_since_epoch().count();
//		auto stop = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimePoint).time_since_epoch().count();
//
//		auto duration = stop - start;
//
//		std::cout << duration << "ms \n";
//	}
//
//private:
//	std::chrono::time_point< std::chrono::high_resolution_clock> startTimePoint;
//};
//
//Timer::Timer()
//{
//	startTimePoint = std::chrono::high_resolution_clock::now();
//}
//
//Timer::~Timer()
//{
//	Stop();
//}


int main(int argc, char* argv[])
{

		GLScene(argc, argv);
		glutMainLoop();

}
