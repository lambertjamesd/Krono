#pragma once

#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace krono
{

class Plane
{
public:
	Plane(void);
	Plane(const Vector3f& normal, float distance);
	Plane(const Vector4f& vectorValue);
	~Plane(void);
private:
	Vector4f mVectorValue;
};

}
