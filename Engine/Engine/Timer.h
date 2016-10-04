#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>
#pragma comment(lib, "winmm.lib")

class Timer
{
public:
	Timer();
	Timer(const Timer& other);
	~Timer();

	void Init();
	void Tick();

	float GetDeltaTime() const;

private:
	unsigned long mPrevTime;
	unsigned long mCurrTime;
	unsigned long mDeltaTime;
};

#endif
