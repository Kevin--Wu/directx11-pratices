#include "Model.h"

Model::Model()
{
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;

	mTexture = nullptr;
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

bool Model::Init(ID3D11Device* device, WCHAR* textureName, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	mBitmapWidth = bitmapWidth;
	mBitmapHeight = bitmapHeight;

	mPrevPosX = -1;
	mPrevPosY = -1;

	Check(InitBuffers(device));

	mTexture = new Texture;
	Check(mTexture->Init(device, textureName));

	return true;
}

void Model::Render(ID3D11DeviceContext* context, int posX, int posY)
{
	UpdateBuffers(context, posX, posY);
	RenderBuffers(context);
}

void Model::Shutdown()
{
	if (mTexture)
	{
		mTexture->Shutdown();
		SafeDelete(mTexture);
	}

	ShutdownBuffers();
}

bool Model::InitBuffers(ID3D11Device* device)
{
	mVertexCount = 6;
	mIndexCount = mVertexCount;

	Vertex* vertices = new Vertex[mVertexCount];
	unsigned long* indices = new unsigned long[mIndexCount];
	
	memset(vertices, 0, sizeof(Vertex) * mVertexCount);

	for (int i = 0; i < mIndexCount; ++i)
		indices[i] = i;

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.ByteWidth = sizeof(Vertex) * mVertexCount;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = vertices;
	vbData.SysMemPitch = 0;
	vbData.SysMemSlicePitch = 0;

	HR(device->CreateBuffer(&vbDesc, &vbData, &mVertexBuffer));

	D3D11_BUFFER_DESC ibDesc;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA ibData;
	ibData.pSysMem = indices;
	ibData.SysMemPitch = 0;
	ibData.SysMemSlicePitch = 0;

	HR(device->CreateBuffer(&ibDesc, &ibData, &mIndexBuffer));

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void Model::UpdateBuffers(ID3D11DeviceContext* context, int posX, int posY)
{
	if ((mPrevPosX == posX) && (mPrevPosY == posY))
		return;

	mPrevPosX = posX;
	mPrevPosY = posY;

	float halfSW = static_cast<float>(mScreenWidth >> 1);
	float halfSH = static_cast<float>(mScreenHeight >> 1);

	float left = -1.0f * halfSW + posX;
	float right = left + mBitmapWidth;
	float top = halfSH - posY;
	float bottom = top - mBitmapHeight;

	Vertex* vertices = new Vertex[mVertexCount];

	vertices[0].pos = XMFLOAT3(left, top, 0.0f);
	vertices[0].tex = XMFLOAT2(0.0f, 0.0f);
	vertices[1].pos = XMFLOAT3(right, bottom, 0.0f);
	vertices[1].tex = XMFLOAT2(1.0f, 1.0f);
	vertices[2].pos = XMFLOAT3(left, bottom, 0.0f);
	vertices[2].tex = XMFLOAT2(0.0f, 1.0f);

	vertices[3].pos = XMFLOAT3(left, top, 0.0f);
	vertices[3].tex = XMFLOAT2(0.0f, 0.0f);
	vertices[4].pos = XMFLOAT3(right, top, 0.0f);
	vertices[4].tex = XMFLOAT2(1.0f, 0.0f);
	vertices[5].pos = XMFLOAT3(right, bottom, 0.0f);
	vertices[5].tex = XMFLOAT2(1.0f, 1.0f);

	D3D11_MAPPED_SUBRESOURCE mappedRes;
	HR(context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes));
	Vertex* dataPtr = (Vertex*)mappedRes.pData;
	memcpy(dataPtr, (void*)vertices, sizeof(Vertex)*mVertexCount);
	context->Unmap(mVertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;
}

void Model::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::ShutdownBuffers()
{
	ReleaseCOM(mVertexBuffer);
	ReleaseCOM(mIndexBuffer);
}

ID3D11ShaderResourceView* Model::GetTexture() const
{
	return mTexture->GetTexture();
}