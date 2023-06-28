#include "Benchmark.h"
#include <iostream>

std::pair<float, float> Benchmark::GetFinalScore()
{
    //return std::pair<float, float>();
    //calculate the standard deviation
    float sum = 0;
    for (auto& time : measured_times)
    {
		sum += time;
	}
    float mean = sum / measured_times.size();
	float variance = 0;
    for (auto& time : measured_times)
    {
		variance += (time - mean) * (time - mean);
	}
	variance /= measured_times.size();
	float std_dev = sqrt(variance);
	return std::pair<float, float>(mean, std_dev);
}


void Benchmark::DisplayResults() {
	//print the results to console
	//print nice line with hyphens
	std::cout << "----------------------------------------" << std::endl;
	std::cout << title << std::endl;
	for (int i = 0; i < final_scores.size(); i++)
	{
		std::cout << "Size: " << sizes[i] << " Average: " << final_scores[i].first * 1000 << " ms " << "| Std Dev: " << final_scores[i].second * 1000 << " ms " << std::endl;
	}
	std::cout << "----------------------------------------" << std::endl;
}

//return true when we still have more to do
bool Benchmark::Advance()
{
	if (done) {
		return false;
	}

	while (current_warmup_cycle < warmup_cycles)
	{
		current_warmup_cycle++;
		return true;
	}

	current_iteration++;
	if (current_iteration >= iterations)
	{
		final_scores.push_back(GetFinalScore());
		measured_times.clear();
		current_iteration = 0;
		current_size_index++;
		current_warmup_cycle = 0;
		if (current_size_index >= sizes.size())
		{
			DisplayResults();
			done = true;
			return false;
		}
	}
	return true;
}

int Benchmark::GetCurrentSize()
{
	return sizes[current_size_index];
}

void Benchmark::Reset()
{
    measured_times.clear();
}

void Benchmark::StartTiming()
{
    timer.start();
}

void Benchmark::StopTiming()
{
	float time = timer.stop();
	if(current_warmup_cycle >= warmup_cycles)
		measured_times.push_back(time);
}