#include "Timer.h"

Timer::Timer()
{
}

Timer::Timer(const Timer& other)
{
}

Timer::~Timer()
{
}

void Timer::Init()
{
	mPrevTime = 0;
	mCurrTime = 0;
	mDeltaTime = 0;
}

float Timer::GetDeltaTime() const
{
	return static_cast<float>(mDeltaTime) / 1000.0f;
}

void Timer::Tick()
{
	mCurrTime = timeGetTime();
	mDeltaTime = mCurrTime - mPrevTime;
	mPrevTime = timeGetTime();
}