#include "Graphics.h"

Graphics::Graphics()
{
	mD3D = nullptr;
	mTimer = nullptr;
	mModel = nullptr;
	mCamera = nullptr;
	mShader = nullptr;
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
	mCamera->SetPosition(0.0f, 0.0f, -10.0f);

	mTimer = new Timer;

	mModel = new Model;
	Check(mModel->Init(mD3D->GetDevice()));

	mShader = new Shader;
	Check(mShader->Init(hwnd, mD3D->GetDevice()));

	return true;
}

void Graphics::Shutdown()
{
	if (mShader)
	{
		mShader->Shutdown();
		SafeDelete(mShader);
	}

	if (mModel)
	{
		mModel->Shutdown();
		SafeDelete(mModel);
	}

	if (mTimer)
	{
		SafeDelete(mTimer);
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
	mTimer->Tick();

	Check(Render());

	return true;
}

bool Graphics::Render()
{
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	static float angle = 0.0f;
	angle += mTimer->GetDeltaTime();
	if (angle >= 6.28f)
		angle = 0.0f;

	mD3D->SetWorldMatrix(XMMatrixRotationY(angle));

	mCamera->Render();
	XMFLOAT4X4 world = mD3D->GetWorldMatrix();
	XMFLOAT4X4 view = mCamera->GetViewMatrix();
	XMFLOAT4X4 proj = mD3D->GetProjMatrix();
	mModel->Render(mD3D->GetDeviceContext());
	mShader->Render(mD3D->GetDeviceContext(), mModel->GetIndexCount(), world, view, proj, mModel->GetTexture());

	mD3D->EndScene();

	return true;
}