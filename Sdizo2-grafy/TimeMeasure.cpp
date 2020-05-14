#include "TimeMeasure.h"

TimeMeasure::TimeMeasure()
{
}

long long TimeMeasure::returnTime()
{
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(end - this->start).count();
}

void TimeMeasure::restart()
{
	this->start = std::chrono::high_resolution_clock::now();
}

TimeMeasure::~TimeMeasure()
{
}
