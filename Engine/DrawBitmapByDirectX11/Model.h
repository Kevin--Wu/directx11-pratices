#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <fstream>
#include <DirectXMath.h>
using namespace DirectX;

#include "Utils.h"
#include "Texture.h"

class Model
{
private:
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

public:
	Model();
	Model(const Model& other);
	~Model();

	bool Init(ID3D11Device* device, WCHAR* textureName, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight);
	void Render(ID3D11DeviceContext* context, int posX, int posY);
	void Shutdown();

	int GetVertexCount() const { return mVertexCount; }
	int GetIndexCount() const { return mIndexCount; }
	ID3D11ShaderResourceView* GetTexture() const;

private:
	bool InitBuffers(ID3D11Device* device);
	void UpdateBuffers(ID3D11DeviceContext* context, int posX, int posY);
	void RenderBuffers(ID3D11DeviceContext* context);
	void ShutdownBuffers();

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount;
	int mIndexCount;

	Texture* mTexture;

	int mScreenWidth, mScreenHeight;
	int mBitmapWidth, mBitmapHeight;
	int mPrevPosX, mPrevPosY;
};

#endif
