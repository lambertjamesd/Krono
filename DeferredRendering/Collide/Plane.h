#pragma once

#include "../Math/Vector3.h"

class Plane
{
public:
	Plane(void);
	Plane(const Vector3f& normal, float distance);
	~Plane(void);
private:
	Vector3f mNormal;
	float mDistance;
};

