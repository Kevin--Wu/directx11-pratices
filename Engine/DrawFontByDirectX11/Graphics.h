#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <windows.h>
#include "D3D.h"
#include "Text.h"
#include "Camera.h"


const bool GRAPHICS_FULL_SCREEN = false;
const bool GRAPHICS_VSYNC_ENABLE = true;
const float GRAPHICS_SCREEN_DEPTH = 1000.0f;
const float GRAPHICS_SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics& other);
	~Graphics();

	bool Init(HWND hwnd, int width, int height);
	bool Frame();
	bool Render();
	void Shutdown();

private:


private:
	D3D* mD3D;
	Text* mText;
	Camera* mCamera;
};
#endif
