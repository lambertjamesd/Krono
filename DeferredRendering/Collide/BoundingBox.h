#pragma once

#include "../Math/Vector3.h"

template <typename T>
class BoundingBox
{
public:
	BoundingBox(void) {};
	~BoundingBox(void) {};

	Vector3<T> min;
	Vector3<T> max;
};

typedef BoundingBox<float> BoundingBoxf;
typedef BoundingBox<int> BoundingBoxi; 

