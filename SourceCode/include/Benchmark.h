#include <vector>

//a benchmarking class to take the average rendering and simulation time, and std deviation and write to CSV
class Benchmark {
	//num iterations
	int iteration = 10;
	//sizes to test
	std::vector<int> sizes = {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000};
	//warm up time 
	float warmup_time = 10.0f;

	//What does this file need to do?
	//It needs to make it easy to run benchmarks
	//What are nice functions?
	//Get iterations()
	//Get next size (for initializing next life);
	//Get warmup_time
	//a start and stop timer function
	//This should add the elapsed time to a list corresponding to size
	//this list should be averaged and std deviationed
};