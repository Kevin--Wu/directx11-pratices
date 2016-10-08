#include "Graphics.h"

Graphics::Graphics()
{
	mD3D = nullptr;
	mText = nullptr;
	mCamera = nullptr;
}

Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Init(HWND hwnd, int width, int height)
{
	mD3D = new D3D;
	Check(mD3D->Init(hwnd, width, height, GRAPHICS_VSYNC_ENABLE, GRAPHICS_FULL_SCREEN, GRAPHICS_SCREEN_DEPTH, GRAPHICS_SCREEN_NEAR));

	mCamera = new Camera;
	mCamera->SetPosition(0.0f, 0.0f, -1.0f);
	mCamera->Render();

	mText = new Text;
	Check(mText->Init(mD3D->GetDevice(), mD3D->GetDeviceContext(), hwnd, width, height, mCamera->GetViewMatrix()));

	return true;
}

void Graphics::Shutdown()
{
	if (mText)
	{
		mText->Shutdown();
		SafeDelete(mText);
	}

	if (mCamera)
	{
		SafeDelete(mCamera);
	}

	if (mD3D)
	{
		mD3D->Shutdown();
		SafeDelete(mD3D);
	}
}

bool Graphics::Frame()
{
	//Check(Render());

	mCamera->SetPosition(0.0f, 0.0f, -10.0f);

	return true;
}

bool Graphics::Render()
{
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	mCamera->Render();
	XMFLOAT4X4 world = mD3D->GetWorldMatrix();
	XMFLOAT4X4 view = mCamera->GetViewMatrix();
	XMFLOAT4X4 ortho = mD3D->GetOrthoMatrix();

	mD3D->TurnZBufferOff();

	mD3D->TurnAlphaBlendOn();

	mText->Render(mD3D->GetDeviceContext(), world, ortho);

	mD3D->TurnAlphaBlendOff();

	mD3D->TurnZBufferOn();

	mD3D->EndScene();

	return true;
}