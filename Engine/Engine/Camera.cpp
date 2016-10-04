#include "Camera.h"

Camera::Camera() : mPosX(0.0f), mPosY(0.0f), mPosZ(0.0f), mRotX(0.0f), mRotY(0.0f), mRotZ(0.0f)
{
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	mPosX = x;
	mPosY = y;
	mPosZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	mRotX = x;
	mRotY = y;
	mRotZ = z;
}

XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(mPosX, mPosY, mPosZ);
}

XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(mRotX, mRotY, mRotZ);
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return mView;
}

void Camera::Render()
{
	XMFLOAT4 up(0.0f, 1.0f, 0.0f, 0.0f);
	XMFLOAT4 pos(mPosX, mPosY, mPosZ, 0.0f);
	XMFLOAT4 lookAt(0.0f, 0.0f, 1.0f, 0.0f);

	float yaw, pitch, roll;
	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = mRotX * 0.0174532925f;
	yaw = mRotY * 0.0174532925f;
	roll = mRotZ * 0.0174532925f;

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	XMVECTOR vUp = XMLoadFloat4(&up);
	XMVECTOR vPos = XMLoadFloat4(&pos);
	XMVECTOR vLookAt = XMLoadFloat4(&lookAt);
	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	vLookAt = XMVector3TransformCoord(vLookAt, rotationMatrix);
	vUp = XMVector3TransformCoord(vUp, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	vLookAt = vPos + vLookAt;

	// Finally create the view matrix from the three updated vectors.
	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(vPos, vLookAt, vUp));
}