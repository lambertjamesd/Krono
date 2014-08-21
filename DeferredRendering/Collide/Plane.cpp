
#include "Plane.h"

Plane::Plane(void) :
	mDistance(0.0f)
{

}

Plane::Plane(const Vector3f& normal, float distance) :
	mNormal(normal),
	mDistance(distance)
{
	
}

Plane::~Plane(void)
{
}
