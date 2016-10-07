#include "Graphics.h"

Graphics::Graphics()
{
	mD3D = nullptr;
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

	mModel = new Model;
	Check(mModel->Init(mD3D->GetDevice(), L"Textures/flare.dds", width, height, 256, 256));

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
	Check(Render());

	return true;
}

bool Graphics::Render()
{
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	static int posX = 0;
	static int posY = 0;

	// our bitmap set to 256*256, screen set to 800*600
	static const int wBorder = 544; // 800 - 256
	static const int hBorder = 344; // 600 - 256

	static int vX = 1;
	static int vY = 1;

	mCamera->Render();
	XMFLOAT4X4 world = mD3D->GetWorldMatrix();
	XMFLOAT4X4 view = mCamera->GetViewMatrix();
	XMFLOAT4X4 ortho = mD3D->GetOrthoMatrix();

	mD3D->TurnZBufferOff();

	mModel->Render(mD3D->GetDeviceContext(), posX, posY);


	mShader->Render(mD3D->GetDeviceContext(), mModel->GetIndexCount(), 
		world, view, ortho, mModel->GetTexture());


	mD3D->TurnZBufferOn();

	mD3D->EndScene();

	posX += vX;
	posY += vY;

	if (posX > wBorder || posX < 0)
		vX = -1*vX;

	if (posY > hBorder || posY < 0)
		vY = -1*vY;

	return true;
}