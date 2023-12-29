#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	float GetTotalTime()const;
	float GetDeltaTime()const;

	void Restart();
	bool Stop();
	bool Start();
	void Tick();
private: 
	double mSecondsPerCount;
	double mDeltaTime;
	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool isrunning = false;
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
};