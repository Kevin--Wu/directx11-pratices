#include "Graphics.h"

Graphics::Graphics()
{
	mD3D = nullptr;
	mText = nullptr;
	mModel = nullptr;
	mLight = nullptr;
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

	mLight = new Light;
	mLight->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	mLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	mLight->SetDiffuseDir(0.0f, 0.0f, 1.0f);
	mLight->SetSpecularPower(32.0f);
	mLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);

	mModel = new Model;
	Check(mModel->Init(mD3D->GetDevice(), "Models/mayaCube.txt", L"Textures/flare.dds"));

	mCamera->Render();
	mText = new Text;
	mText->Init(mD3D->GetDevice(), mD3D->GetDeviceContext(), hwnd, width, height, "Fonts/fontdata.txt", L"Fonts/font.dds", 16, mCamera->GetViewMatrix());

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

	if (mText)
	{
		mText->Shutdown();
		SafeDelete(mText);
	}

	if (mLight)
	{
		SafeDelete(mLight);
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

bool Graphics::Frame(float dt)
{
	static float angle = 0.0f;
	angle += dt;
	if (angle >= 6.28f)
		angle = 0.0f;

	XMMATRIX rotate = XMMatrixRotationX(angle) * XMMatrixRotationY(angle) * XMMatrixRotationZ(angle);
	mD3D->SetWorldMatrix(rotate);

	mFrameTime = static_cast<int>(1 / dt);

	return true;
}

bool Graphics::Render()
{
	mD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	mCamera->Render();
	XMFLOAT4X4 world = mD3D->GetWorldMatrix();
	XMFLOAT4X4 view = mCamera->GetViewMatrix();
	XMFLOAT4X4 proj = mD3D->GetProjMatrix();

	mModel->Render(mD3D->GetDeviceContext());

	XMMATRIX scale = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	XMMATRIX offset = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX newWorld = XMLoadFloat4x4(&world) * scale * offset;
	XMStoreFloat4x4(&world, newWorld);

	Check(mShader->Render(mD3D->GetDeviceContext(), mModel->GetIndexCount(), 
		world, view, proj, mModel->GetTexture(), 
		mLight->GetAmbientColor(), mLight->GetDiffuseColor(), mLight->GetDiffuseDir(),
		mLight->GetSpecularPower(), mLight->GetSpecularColor(), mCamera->GetPosition()));

	mD3D->TurnDepthOff();
	mD3D->TurnBlendOn();

	XMFLOAT4X4 I;
	XMStoreFloat4x4(&I, XMMatrixIdentity());

	char fpsStr[16] = "FPS:";
	char cpuStr[16] = "CPU:";
	char tempStr[8];

	int cpuRate = 1;

	// In _itoa_s, parameter '10' means we use Decimal System
	_itoa_s(mFrameTime, tempStr, 10);
	strcat_s(fpsStr, tempStr);

	_itoa_s(cpuRate, tempStr, 10);
	strcat_s(cpuStr, tempStr);
	strcpy_s(tempStr, "%");
	strcat_s(cpuStr, tempStr);
	
	Check(mText->Render(mD3D->GetDeviceContext(), fpsStr, 20, 20, XMFLOAT3(1.0f, 0.0f, 0.0f), I, mD3D->GetOrthoMatrix()));
	Check(mText->Render(mD3D->GetDeviceContext(), cpuStr, 20, 40, XMFLOAT3(1.0f, 0.0f, 0.0f), I, mD3D->GetOrthoMatrix()));

	mD3D->TurnBlendOff();
	mD3D->TurnDepthOn();

	mD3D->EndScene();

	return true;
}