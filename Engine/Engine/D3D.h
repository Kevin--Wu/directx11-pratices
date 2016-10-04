#ifndef _D3D_H_
#define _D3D_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#include "Utils.h"

class D3D
{
public:
	D3D();
	D3D(const D3D& other);
	~D3D();

	bool Init(HWND hwnd, int screenWidth, int screenHeight, bool vsync, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	XMFLOAT4X4 GetWorldMatrix() const;
	XMFLOAT4X4 GetProjMatrix() const;
	XMFLOAT4X4 GetOrthoMatrix() const;
	XMMATRIX GetWorldMatrixXM() const;
	XMMATRIX GetProjMatrixXM() const;
	XMMATRIX GetOrthoMatrixXM() const;
	void GetVideoCardInfo(char*, int&) const;

private:
	bool mVsyncEnabled;
	int mVideoCardMemory;
	char mVideoCardDescription[128];

	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RasterizerState* mRasterState;

	XMFLOAT4X4 mProj;
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mOrtho;
};

#endif
