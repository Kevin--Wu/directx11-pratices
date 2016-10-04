#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(const Camera& other);
	~Camera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT3   GetPosition();
	XMFLOAT3   GetRotation();

	void Render();

private:
	float mPosX, mPosY, mPosZ;
	float mRotX, mRotY, mRotZ;
	XMFLOAT4X4 mView;
};
#endif
