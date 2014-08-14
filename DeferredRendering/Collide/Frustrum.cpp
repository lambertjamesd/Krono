
#include "Frustrum.h"


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

}