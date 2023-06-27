#include <vector>
#include "Stopwatch.h"

//a benchmarking class to take the average rendering and simulation time, and std deviation and write to CSV
class Benchmark {
	Stopwatch timer;

	std::vector<float> measured_times;

	std::vector<std::pair<float, float>> final_scores;

	std::string title = "Benchmark";

	const int iterations = 10;
	int current_iteration = 0;
	int current_size_index = 0;

	const int warmup_cycles = 20;
	int current_warmup_cycle = 0;

	bool done = false;

	//returns the average measured time and the standard deviation
	std::pair<float, float> GetFinalScore();

	std::vector<int> sizes = { 1000, 2000, 3000, 4000, 5000 };

	void DisplayResults();

	void Reset();

public:

	//contructor
	Benchmark(std::string title, std::vector<int> sizes, int iterations, int warmup_cycles):
		title(title), sizes(sizes), iterations(iterations), warmup_cycles(warmup_cycles) {}

	bool Advance();

	void StartTiming();

	void StopTiming();

	int GetCurrentSize();

	bool IsDone() { return done; }

};