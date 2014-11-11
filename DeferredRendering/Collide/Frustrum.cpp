
#include "Frustrum.h"

namespace krono
{

Frustrum::Frustrum(const Matrix4f& projViewMatrix)
{
	BuildFrustrum(projViewMatrix, projViewMatrix.Inverse());
}

Frustrum::Frustrum(void)
{
}


Frustrum::~Frustrum(void)
{
}

void Frustrum::BuildFrustrum(const Matrix4f& projViewMatrix, const Matrix4f& projViewInvMatrix)
{
	mFaces[Right] = Plane(Vector4f(-1.0f, 0.0f, 0.0f, 1.0f) * projViewInvMatrix);
	mFaces[Left] = Plane(Vector4f(1.0f, 0.0f, 0.0f, 1.0f) * projViewInvMatrix);
	mFaces[Top] = Plane(Vector4f(0.0f, -1.0f, 0.0f, 1.0f) * projViewInvMatrix);
	mFaces[Bottom] = Plane(Vector4f(0.0f, 1.0f, 0.0f, 1.0f) * projViewInvMatrix);
	mFaces[Back] = Plane(Vector4f(0.0f, 0.0f, -1.0f, 1.0f) * projViewInvMatrix);
	mFaces[Front] = Plane(Vector4f(0.0f, 0.0f, 1.0f, 1.0f) * projViewInvMatrix);
}

}
