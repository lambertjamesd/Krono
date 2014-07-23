#pragma once

#include "../Math/Matrix.h"
#include "../Math/Vector3.h"
#include "Plane.h"
#include "BoundingBox.h"

class Frustrum
{
public:
	enum Face
	{
		Right,
		Left,
		Top,
		Bottom,
		Front,
		Back,

		FaceCount
	};

	Frustrum(void);
	Frustrum(const Matrix4f& projViewMatrix);
	~Frustrum(void);

	bool DoesCollide(const BoundingBoxf& boundingBox) const;
private:
	void BuildFrustrum(const Matrix4f& projViewMatrix, const Matrix4f& projViewInvMatrix);

	static const int CornerCount = 8;

	Plane mFaces[FaceCount];
	Vector3f mWorldPoints[CornerCount];
};

