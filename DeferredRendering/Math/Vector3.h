#pragma once
#include <cmath>
#include "Math.h"
#include "Vector2.h"

namespace krono
{

template <typename T>
class Vector3
{
public:
	Vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
	Vector3(const Vector2<T>& xy, const T& z) : x(xy.x), y(xy.y), z(z) {}
	Vector3(const T& x, const Vector2<T>& yz) : x(x), y(yz.y), z(yz.z) {}
	Vector3(void) : x(T()), y(T()), z(T()) {}
	~Vector3(void) {}

	bool operator==(const Vector3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const Vector3& other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}

	bool IsZero() const
	{
		return x == Constant<T>::Zero && y == Constant<T>::Zero && z == Constant<T>::Zero;
	}

	Vector3 Normalized() const
	{
		Vector3 result(*this);
		result.Normalize();
		return result;
	}

	void Normalize()
	{
		T lengthSqrd = x * x + y * y + z * z;

		if (lengthSqrd > Constant<T>::Zero)
		{
			T lengthInv = Constant<T>::One / Math<T>::Sqrt(lengthSqrd);

			x = x * lengthInv;
			y = y * lengthInv;
			z = z * lengthInv;
		}
	}
	
	template <typename R>
	operator Vector3<R>() const
	{
		return Vector3<R>((R)x, (R)y, (R)z);
	}

	T x, y, z;
};

template <typename T>
Vector3<T> operator+ (const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T>
Vector3<T> operator- (const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T>
Vector3<T> operator- (const Vector3<T>& a)
{
	return Vector3<T>(-a.x, -a.y, -a.z);
}

template <typename T>
Vector3<T> operator* (const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <typename T>
Vector3<T> operator* (const T& a, const Vector3<T>& b)
{
	return Vector3<T>(a * b.x, a * b.y, a * b.z);
}

template <typename T>
Vector3<T> operator* (const Vector3<T>& a, const T& b)
{
	return Vector3<T>(a.x * b, a.y * b, a.z * b);
}

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;

}
