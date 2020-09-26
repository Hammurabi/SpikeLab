#include "Time.h"
#include <chrono>

double getTime()
{
	const double NanoSecondsPerSecond = 1000000000.0;
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() / NanoSecondsPerSecond;
}
