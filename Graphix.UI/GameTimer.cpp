#include "GameTimer.h"

void GameTimer::Start()
{
	QueryPerformanceCounter(&_start);
	QueryPerformanceFrequency(&_freq);
	_then = _start;
}

void GameTimer::Stop()
{
}

double GameTimer::GetElapsedMilliseconds()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double elapsed = (now.QuadPart - _then.QuadPart) / (_freq.QuadPart / 1000.0);
	_then = now;
	return elapsed;
}
