#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int iCmdShow)
{
	System* pSystem = new System;
	Check(pSystem->Init());

	pSystem->Run();
	pSystem->Shutdown();
	SafeDelete(pSystem);

	return 0;
}