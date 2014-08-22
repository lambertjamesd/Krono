#pragma once

#include "Vector3.h"

namespace krono
{

template <typename T>
class Vector4
{
public:
	Vector4(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {}
	Vector4(const Vector3<T>& xyz, const T& w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
	Vector4(void) : x(T()), y(T()), z(T()), w(T()) {}
	~Vector4(void) {}
	
	template <typename R>
	operator Vector4<R>() const
	{
		return Vector4<R>((R)x, (R)y, (R)z, (R)w);
	}

	T x, y, z, w;
};

typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;

}
