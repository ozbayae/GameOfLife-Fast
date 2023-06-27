#pragma once
#include <chrono>
#include <string>

struct Timer
{
	Timer() { reset(); }
	float elaspsed() const {
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - start);
		return (float)time_span.count();
	}
	void reset() { start = std::chrono::high_resolution_clock::now(); }
	std::chrono::high_resolution_clock::time_point start;
};

class Stopwatch {
	Timer timer;

public:
	void start();
	void stop(std::string message);
	float stop();
};