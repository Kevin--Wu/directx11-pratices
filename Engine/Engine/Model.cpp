#include "Model.h"

Model::Model()
{
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
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

	return true;
}

void Model::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

void Model::Shutdown()
{
	ShutdownBuffers();
}

bool Model::InitBuffers(ID3D11Device* device)
{
	Vertex* vertices;
	unsigned long* indices;

	mVertexCount = 3;
	mIndexCount = 3;

	vertices = new Vertex[mVertexCount];
	indices = new unsigned long[mIndexCount];


	vertices[0].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].pos = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].pos = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

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