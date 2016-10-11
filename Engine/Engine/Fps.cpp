#include "Fps.h"

Fps::Fps()
{
}

Fps::Fps(const Fps& other)
{
}

Fps::~Fps()
{
}

void Fps::Init()
{
	mCount = 0;
	mFps = 0;
	mStartTime = timeGetTime();
}

void Fps::Frame()
{
	++mCount;

	if (timeGetTime() >= (mStartTime + 1000))
	{
		mStartTime = timeGetTime();
		mFps = mCount;
		mCount = 0;
	}
}

int Fps::GetFps() const
{
	return mFps;
}