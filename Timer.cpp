#include "Timer.h"

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

float Timer::DeltaTime()const
{
	return (float)mDeltaTime;
}
void Timer::Restart()
{
	__int64 currTime;
	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mPausedTime = false;
	isrunning = true;
	start = std::chrono::high_resolution_clock::now();
}
bool Timer::Stop()
{
	if (!isrunning)
		return false;
	else
	{
		__int64 currTime;
		mStopTime = currTime;
		stop = std::chrono::high_resolution_clock::now();
		isrunning = false;
		return true;
	}
}
bool Timer::Start()
{
	__int64 startTime;
	if (isrunning)
	{
		return false;
	}
	else
	{
		mPausedTime += (startTime - mStopTime);
		mPrevTime = startTime;
		mStopTime = 0;
		mPausedTime = false;
		start = std::chrono::high_resolution_clock::now();
		isrunning = true;
		return true;
	}
}
void Timer::Tick()
{
	if (mStopTime)
	{
		mDeltaTime = 0.0;
		return;
	}
	__int64 currTime;
	
	mCurrTime = currTime;

	// Time difference between this frame and the previous.
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	// Prepare for next frame.
	mPrevTime = mCurrTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}