#pragma once
#include <windows.h>
class GameTimer
{
public:
	void Start();
	void Stop();
	double GetElapsedMilliseconds();

private:
	LARGE_INTEGER _start;
	LARGE_INTEGER _freq;
	LARGE_INTEGER _end;
	LARGE_INTEGER _then;

};

