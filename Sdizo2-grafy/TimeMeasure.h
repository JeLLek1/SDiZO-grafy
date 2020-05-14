#pragma once
#include <chrono>
class TimeMeasure
{
private:
	std::chrono::high_resolution_clock::time_point start;
public:
	TimeMeasure();
	long long returnTime();
	void restart();
	~TimeMeasure();
};

