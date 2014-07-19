#include "stdafx.h"
#include "Plane.h"


Plane::Plane(const Vector3f& normal, float distance) :
	mNormal(normal),
	mDistance(distance)
{
	
}

Plane::~Plane(void)
{
}
