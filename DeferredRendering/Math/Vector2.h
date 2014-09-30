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

	T Length() const
	{
		return Math<T>::Sqrt(x * x + y * y);
	}

	T LengthSqrd() const
	{
		return x * x + y * y;
	}

	Vector2 Normalized() const
	{
		Vector2 result(*this);
		result.Normalize();
		return result;
	}

	void Normalize()
	{
		T lengthSqrd = x * x + y * y;

		if (lengthSqrd > Constant<T>::Zero)
		{
			T lengthInv = Constant<T>::One / Math<T>::Sqrt(lengthSqrd);

			x = x * lengthInv;
			y = y * lengthInv;
		}
	}

	T Dot(const Vector2& other) const
	{
		return x * other.x + y * other.y;
	}

	Vector2 ProjectOnto(const Vector2& normal) const
	{
		return normal * Dot(normal) / normal.LengthSqrd();
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
Vector2<T>& operator+= (Vector2<T>& a, const Vector2<T>& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

template <typename T>
Vector2<T> operator- (const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.x - b.x, a.y - b.y);
}

template <typename T>
Vector2<T>& operator-= (Vector2<T>& a, const Vector2<T>& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
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
Vector2<T>& operator*= (Vector2<T>& a, const Vector2<T>& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

template <typename T>
Vector2<T>& operator*= (Vector2<T>& a, const T& b)
{
	a.x *= b;
	a.y *= b;
	return a;
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
Vector2<T>& operator/= (Vector2<T>& a, const Vector2<T>& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

template <typename T>
Vector2<T>& operator/= (Vector2<T>& a, const T& b)
{
	T invB = Constant<T>::One / b;

	a.x *= invB;
	a.y *= invB;
	return a;
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
