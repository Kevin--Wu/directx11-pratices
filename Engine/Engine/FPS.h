#ifndef _FPS_H_
#define _FPS_H_

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>

class Fps
{
public:
	Fps();
	Fps(const Fps& other);
	~Fps();

	void Init();
	void Frame();
	int GetFps() const;

private:
	int mFps, mCount;
	unsigned long mStartTime;
};

#endif