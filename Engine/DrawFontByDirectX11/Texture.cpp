#include "Texture.h"

Texture::Texture()
{
	mMapSRV = nullptr;
}

Texture::Texture(const Texture& other)
{
}

Texture::~Texture()
{
}

bool Texture::Init(ID3D11Device* device, WCHAR* fileName)
{
//	ID3D11Resource* texRes = nullptr;
//	HR(DirectX::CreateDDSTextureFromFile(device, fileName, &texRes, &mMapSRV));

	HR(D3DX11CreateShaderResourceViewFromFile(device, fileName, 0, 0, &mMapSRV, 0));

	return true;
}

ID3D11ShaderResourceView* Texture::GetTexture() const
{
	return mMapSRV;
}

void Texture::Shutdown()
{
	ReleaseCOM(mMapSRV);
}