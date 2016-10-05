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

bool Model::Init(ID3D11Device* device)
{
	Check(InitBuffers(device));

	mTexture = new Texture;
	Check(mTexture->Init(device, L"Textures/flare.dds"));

	return true;
}

void Model::Render(ID3D11DeviceContext* context)
{
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
	Vertex* vertices;
	unsigned long* indices;

	mVertexCount = 24;
	mIndexCount = 36;

	vertices = new Vertex[mVertexCount];
	indices = new unsigned long[mIndexCount];

	float w = 1.0f;
	float h = 1.0f;
	float d = 1.0f;

	// front face
	vertices[0] = { XMFLOAT3(-w, -h,  d), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) };
	vertices[1] = { XMFLOAT3(-w,  h,  d), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) };
	vertices[2] = { XMFLOAT3( w,  h,  d), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) };
	vertices[3] = { XMFLOAT3( w, -h,  d), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) };

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// right face
	vertices[4] = { XMFLOAT3( w, -h,  d), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) };
	vertices[5] = { XMFLOAT3( w,  h,  d), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) };
	vertices[6] = { XMFLOAT3( w,  h, -d), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) };
	vertices[7] = { XMFLOAT3( w, -h, -d), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) };

	indices[6] = 4; indices[7] = 5;  indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// back face
	vertices[8]  = { XMFLOAT3( w, -h, -d), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) };
	vertices[9]  = { XMFLOAT3( w,  h, -d), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) };
	vertices[10] = { XMFLOAT3(-w,  h, -d), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) };
	vertices[11] = { XMFLOAT3(-w, -h, -d), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) };

	indices[12] = 8; indices[13] = 10;  indices[14] = 9;
	indices[15] = 8; indices[16] = 11; indices[17] = 10;

	// left face
	vertices[12] = { XMFLOAT3(-w, -h, -d), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) };
	vertices[13] = { XMFLOAT3(-w,  h, -d), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) };
	vertices[14] = { XMFLOAT3(-w,  h,  d), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) };
	vertices[15] = { XMFLOAT3(-w, -h,  d), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) };

	indices[18] = 12; indices[19] = 14;  indices[20] = 13;
	indices[21] = 12; indices[22] = 15;  indices[23] = 14;

	// top face
	vertices[16] = { XMFLOAT3(-w,  h,  d), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) };
	vertices[17] = { XMFLOAT3(-w,  h, -d), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) };
	vertices[18] = { XMFLOAT3( w,  h, -d), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) };
	vertices[19] = { XMFLOAT3( w,  h,  d), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) };

	indices[24] = 16; indices[25] = 17;  indices[26] = 18;
	indices[27] = 16; indices[28] = 18;  indices[29] = 19;

	// bottom face
	vertices[20] = { XMFLOAT3(-w, -h,  d), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) };
	vertices[21] = { XMFLOAT3(-w, -h, -d), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) };
	vertices[22] = { XMFLOAT3( w, -h, -d), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) };
	vertices[23] = { XMFLOAT3( w, -h,  d), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) };

	indices[24] = 20; indices[25] = 21;  indices[26] = 22;
	indices[27] = 20; indices[28] = 22;  indices[29] = 23;

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(Vertex) * mVertexCount;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
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