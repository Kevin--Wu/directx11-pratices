#include "light.h"

Light::Light()
{
}

Light::Light(const Light& other)
{
}

Light::~Light()
{
}

void Light::SetAmbientColor(float r, float g, float b, float a)
{
	mAmbientColor = XMFLOAT4(r, g, b, a);
}

void Light::SetDiffuseColor(float r, float g, float b, float a)
{
	mDiffuseColor = XMFLOAT4(r, g, b, a);
}

void Light::SetDiffuseDir(float x, float y, float z)
{
	mDiffuseDir = XMFLOAT3(x, y, z);
}

XMFLOAT4 Light::GetAmbientColor() const
{
	return mAmbientColor;
}

XMFLOAT4 Light::GetDiffuseColor() const
{
	return mDiffuseColor;
}

XMFLOAT3 Light::GetDiffuseDir() const
{
	return mDiffuseDir;
}