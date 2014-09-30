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
	Vector4(const T& x, const Vector3<T>& yzw) : x(x), y(yzw.y), z(yzw.z), w(yzw.w) {}
	Vector4(const Vector2<T>& xy, const T& z, const T& w) : x(xy.x), y(xy.y), z(z), w(w) {}
	Vector4(const T& x, const Vector2<T>& yz, const T& w) : x(x), y(yz.y), z(yz.z), w(w) {}
	Vector4(const T& x, const T& y, const Vector2<T>& zw) : x(x), y(y), z(zw.z), w(zw.w) {}
	Vector4(const Vector2<T>& xy, const Vector2<T>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
	Vector4(void) : x(T()), y(T()), z(T()), w(T()) {}
	~Vector4(void) {}

	Vector3<T> XYZ() const
	{
		return Vector3<T>(x, y, z); 
	}

	Vector2<T> XY() const
	{
		return Vector2<T>(x, y); 
	}
	
	template <typename R>
	operator Vector4<R>() const
	{
		return Vector4<R>((R)x, (R)y, (R)z, (R)w);
	}

	T x, y, z, w;
};

typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;

template <typename T>
Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b)
{
	return Vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T>
Vector4<T>& operator+=(Vector4<T>& a, const Vector4<T>& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;
	return a;
}

}
