#include "Cpu.h"

Cpu::Cpu()
{
}

Cpu::Cpu(const Cpu& other)
{
}

Cpu::~Cpu()
{
}

void Cpu::Init()
{
	mCanReadCpu = true;

	PDH_STATUS pdhStatus;
	pdhStatus = PdhOpenQuery(NULL, 0, &mQueryHandle);
	if (pdhStatus != ERROR_SUCCESS)
		mCanReadCpu = false;

	pdhStatus = PdhAddCounter(mQueryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &mCounterHandle);
	if (pdhStatus != ERROR_SUCCESS)
		mCanReadCpu = false;

	mLastSampleTime = GetTickCount();
	mCpuUsage = 0;
}

void Cpu::Shutdown()
{
	if (mCanReadCpu)
		PdhCloseQuery(mQueryHandle);
}

void Cpu::Frame()
{
	if (mCanReadCpu)
	{
		if (GetTickCount() >= (mLastSampleTime + 1000))
		{
			mLastSampleTime = GetTickCount();

			PdhCollectQueryData(mQueryHandle);
			PDH_FMT_COUNTERVALUE value;
			PdhGetFormattedCounterValue(mCounterHandle, PDH_FMT_LONG, NULL, &value);
			mCpuUsage = value.longValue;
		}
	}
}

int Cpu::GetCpuPerCentage() const
{
	int usage;
	if (mCanReadCpu)
		usage = static_cast<int>(mCpuUsage);
	else
		usage = 0;

	return usage;
}