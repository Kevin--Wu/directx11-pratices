#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::Frustum(const Frustum& other)
{
}

Frustum::~Frustum()
{
}

void Frustum::ConstructFrustum(float screenDepth, XMFLOAT4X4 proj, XMFLOAT4X4 view)
{
	// Update proj matrix's far plane = screenDepth
	// nz means near plane's depth of frustum
	float nz = -(proj(4, 3) / proj(3, 3));
	float r = screenDepth / (screenDepth - nz);
	proj(3, 3) = r;
	proj(4, 3) = -r*nz;

	// frustum matrix
	XMMATRIX f = XMLoadFloat4x4(&view) * XMLoadFloat4x4(&proj);
	XMFLOAT4X4 M;
	XMStoreFloat4x4(&M, f);

	//
	// Left
	//
	mPlanes[0].x = M(0, 3) + M(0, 0);
	mPlanes[0].y = M(1, 3) + M(1, 0);
	mPlanes[0].z = M(2, 3) + M(2, 0);
	mPlanes[0].w = M(3, 3) + M(3, 0);

	//
	// Right
	//
	mPlanes[1].x = M(0, 3) - M(0, 0);
	mPlanes[1].y = M(1, 3) - M(1, 0);
	mPlanes[1].z = M(2, 3) - M(2, 0);
	mPlanes[1].w = M(3, 3) - M(3, 0);

	//
	// Bottom
	//
	mPlanes[2].x = M(0, 3) + M(0, 1);
	mPlanes[2].y = M(1, 3) + M(1, 1);
	mPlanes[2].z = M(2, 3) + M(2, 1);
	mPlanes[2].w = M(3, 3) + M(3, 1);

	//
	// Top
	//
	mPlanes[3].x = M(0, 3) - M(0, 1);
	mPlanes[3].y = M(1, 3) - M(1, 1);
	mPlanes[3].z = M(2, 3) - M(2, 1);
	mPlanes[3].w = M(3, 3) - M(3, 1);

	//
	// Near
	//
	mPlanes[4].x = M(0, 2);
	mPlanes[4].y = M(1, 2);
	mPlanes[4].z = M(2, 2);
	mPlanes[4].w = M(3, 2);

	//
	// Far
	//
	mPlanes[5].x = M(0, 3) - M(0, 2);
	mPlanes[5].y = M(1, 3) - M(1, 2);
	mPlanes[5].z = M(2, 3) - M(2, 2);
	mPlanes[5].w = M(3, 3) - M(3, 2);

	// Normalize the plane equations.
	for (int i = 0; i < 6; ++i)
	{
		XMVECTOR v = XMPlaneNormalize(XMLoadFloat4(&mPlanes[i]));
		XMStoreFloat4(&mPlanes[i], v);
	}
}

bool Frustum::CheckPoint(XMFLOAT3 point)
{
	XMVECTOR v = XMLoadFloat3(&point);
	for (int i = 0; i < 6; ++i)
	{
		float dot = XMPlaneDotCoord(XMLoadFloat4(&mPlanes[i]), v).m128_f32[0];
		if (dot < 0.0f)
			return false;
	}
	return true;
}

bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	return true;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	return true;
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	return true;
}
