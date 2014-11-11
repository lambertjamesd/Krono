#pragma once

#include "Math/Vector3.h"

namespace krono
{

template <typename T>
class BoundingBox
{
public:
	BoundingBox(void) {};
	BoundingBox(const Vector3<T>& minimum, const Vector3<T>& maximum) : minimum(minimum), maximum(maximum) {};
	~BoundingBox(void) {};

	BoundingBox Union(const BoundingBox& other)
	{
		return BoundingBox(Min(minimum, other.minimum), Max(maximum, other.maximum));
	}

	Vector3<T> minimum;
	Vector3<T> maximum;
};

typedef BoundingBox<float> BoundingBoxf;
typedef BoundingBox<int> BoundingBoxi; 

}
