#pragma once
#include <cmath>
#include "Math.h"

namespace krono
{

template <typename T>
class Vector2
{
public:
	Vector2(const T& x, const T& y) : x(x), y(y) {}
	Vector2(void) : x(T()), y(T()) {}
	~Vector2(void) {}

	bool operator==(const Vector2& other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vector2& other) const
	{
		return x != other.x || y != other.y;
	}

	bool IsZero() const
	{
		return x == Constant<T>::Zero && y == Constant<T>::Zero;
	}

	template <typename R>
	operator Vector2<R>() const
	{
		return Vector2<R>((R)x, (R)y);
	}
	T x, y;
};

template <typename T>
Vector2<T> operator+ (const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.x + b.x, a.y + b.y);
}

template <typename T>
Vector2<T> operator- (const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.x - b.x, a.y - b.y);
}

template <typename T>
Vector2<T> operator- (const Vector2<T>& a)
{
	return Vector2<T>(-a.x, -a.y);
}

template <typename T>
Vector2<T> operator* (const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
Vector2<T> operator* (const T& a, const Vector2<T>& b)
{
	return Vector2<T>(a * b.x, a * b.y);
}

template <typename T>
Vector2<T> operator* (const Vector2<T>& a, const T& b)
{
	return Vector2<T>(a.x * b, a.y * b);
}

template <typename T>
Vector2<T> operator/ (const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.x / b.x, a.y / b.y);
}

template <typename T>
Vector2<T> operator/ (const Vector2<T>& a, const T& b)
{
	return a * (Constant<T>::One / b);
}

template <typename T>
Vector2<T> operator/ (const T& a, const Vector2<T>& b)
{
	return Vector2<T>(a / b.x, a / b.y);
}

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector2i Size;

}
