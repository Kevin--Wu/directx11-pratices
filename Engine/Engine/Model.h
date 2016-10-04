#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;

#include "Utils.h"

class Model
{
private:
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

public:
	Model();
	Model(const Model& other);
	~Model();

	bool Init(ID3D11Device* device);
	void Render(ID3D11DeviceContext* context);
	void Shutdown();

	int GetVertexCount() { return mVertexCount; }
	int GetIndexCount() { return mIndexCount; }

private:
	bool InitBuffers(ID3D11Device* device);
	void RenderBuffers(ID3D11DeviceContext* context);
	void ShutdownBuffers();

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount;
	int mIndexCount;
};

#endif
