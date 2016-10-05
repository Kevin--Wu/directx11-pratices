#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <DirectXMath.h>
using namespace DirectX;

class Light
{
public:
	Light();
	Light(const Light& other);
	~Light();

	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDiffuseDir(float x, float y, float z);

	XMFLOAT4 GetDiffuseColor() const;
	XMFLOAT3 GetDiffuseDir() const;

private:
	XMFLOAT4 mDiffuseColor;
	XMFLOAT3 mDiffuseDir;
};

#endif