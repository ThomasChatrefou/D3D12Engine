#include "PreCompiledHeaders.h"
#include "Timer.h"

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

float Timer::GetTotalTime()const
{
	return (float)mCurrTime;
}
float Timer::GetDeltaTime()const
{
	return (float)mDeltaTime;
}
void Timer::Restart()
{
	__int64 currTime=0;
	mBaseTime = currTime;
	mStopTime = 0;
	mPausedTime = false;
	isrunning = true;
	start = std::chrono::high_resolution_clock::now();
	mPrevTime = std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch()).count();
	mCurrTime = 0;
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
		mStopTime = 0;
		mPausedTime = false;
		start = std::chrono::high_resolution_clock::now();
		mPrevTime = std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch()).count();
		isrunning = true;
		mCurrTime = 0;
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

	auto currChronoTime = std::chrono::high_resolution_clock::now();
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(currChronoTime.time_since_epoch());
	auto currChronoTimeInSec = sec.count();

	// Time difference between this frame and the previous.
	mDeltaTime = (currChronoTimeInSec - mPrevTime) * mSecondsPerCount;

	// Prepare for next frame.
	mPrevTime = currChronoTimeInSec;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}

	mCurrTime += mDeltaTime;
}