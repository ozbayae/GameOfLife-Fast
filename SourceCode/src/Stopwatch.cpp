#include "Stopwatch.h"
#include <iostream>

void Stopwatch::start()
{
	timer.reset();
}

void Stopwatch::stop(std::string message)
{
	//display message and elapsed time as miliseconds 
	std::cout << message << " --- " << timer.elaspsed() * 1000 << "ms" << std::endl;
}
