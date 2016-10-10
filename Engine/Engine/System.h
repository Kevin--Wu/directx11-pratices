#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "Input.h"
#include "Sound.h"
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

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void InitWindow(/*out*/int& width, /*out*/int& height);
	void ShutdownWindow();

private:
	HWND mhWnd;
	WCHAR* mAppName;
	HINSTANCE mhInstance;

	Input* mInput;
	Sound* mSound;
	Graphics* mGraphics;
};

static System* gApp = nullptr;
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

#endif