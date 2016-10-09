#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d11.h>
#include "Utils.h"
#include "Third/DDSTextureLoader.h"  // a light-weight dds loader, but it seems not support font.dds

class Texture
{
public:
	Texture();
	Texture(const Texture& other);
	~Texture();

	bool Init(ID3D11Device* device, WCHAR* fileName);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture() const;

private:
	ID3D11ShaderResourceView* mMapSRV;
};

#endif