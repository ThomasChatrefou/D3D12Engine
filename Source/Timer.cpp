#include "PreCompiledHeaders.h"
#include "../Timer.h"
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
	__int64 currTime=0;
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
		__int64 currTime =0;
		mStopTime = currTime;
		stop = std::chrono::high_resolution_clock::now();
		isrunning = false;
		return true;
	}
}
bool Timer::Start()
{
	__int64 startTime = 0;
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
	auto Currtime = std::chrono::high_resolution_clock::now();
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(Currtime.time_since_epoch());
	currTime = sec.count();

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