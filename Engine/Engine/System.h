#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "Input.h"
#include "Graphics.h"

class System
{
public:
	System();
	System(const System& other);
	~System();

	bool Init();
	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitWindow(int&, int&);
	void ShutdownWindow();

private:
	LPCWSTR mAppName;
	HINSTANCE mhInstance;
	HWND mhWnd;

	Input* mInput;
	Graphics* mGraphics;
};

static System* gApp = nullptr;
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#endif