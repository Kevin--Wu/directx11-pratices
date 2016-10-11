#ifndef _CPU_H_
#define _CPU_H_

#pragma comment(lib, "pdh.lib")

#include <pdh.h>

class Cpu
{
public:
	Cpu();
	Cpu(const Cpu& other);
	~Cpu();

public:
	void Init();
	void Frame();
	void Shutdown();
	int GetCpuPerCentage() const;

private:
	bool mCanReadCpu;
	HQUERY mQueryHandle;
	HCOUNTER mCounterHandle;
	unsigned long mLastSampleTime;
	long mCpuUsage;
};

#endif