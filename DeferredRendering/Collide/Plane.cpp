
#include "Plane.h"

namespace krono
{

Plane::Plane(void)
{

}

Plane::Plane(const Vector3f& normal, float distance) :
	mVectorValue(normal, distance)
{
	
}

Plane::Plane(const Vector4f& vectorValue) :
	mVectorValue(vectorValue)
{

}

Plane::~Plane(void)
{
}

}
