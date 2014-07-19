#pragma once

#include "Vector3.h"

class Plane
{
public:
	Plane(const Vector3f& normal, float distance);
	~Plane(void);
private:
	Vector3f mNormal;
	float mDistance;
};

