#pragma once

#include "Math.h"
#include "Vector3.h"
#include "Angle.h"

template <typename T>
class Quaternion
{
public:
	Quaternion(T w, T x, T y, T z) : x(x), y(y), z(z), w(w) {}
	Quaternion(const Vector3<T>& axis, Radians<T> angle)
	{
		T cosHalfAngle = Math<T>::Cos((T)angle / 2);
		T sinHalfAngle = Math<T>::Sin((T)angle / 2);

		Vector3<T> normalizedAxis = axis.Normalized();

		w = cosHalfAngle;
		x = sinHalfAngle * normalizedAxis.x;
		y = sinHalfAngle * normalizedAxis.y;
		z = sinHalfAngle * normalizedAxis.z;
	}

	Quaternion operator*(const Quaternion& other) const
	{
		return Quaternion(
			w * other.w - x * other.x - y * other.y - z * other.z,
			w * other.x + other.w * x + y * other.z - other.y * z,
			w * other.y + other.w * y + z * other.x - other.z * x,
			w * other.z + other.w * z + x * other.y - other.x * y);
	}

	Quaternion operator+(const Quaternion& other) const
	{
		return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
	}

	Quaternion operator-(const Quaternion& other) const
	{
		return Quaternion(w - other.w, x - other.x, y - other.y, z - other.z);
	}

	Quaternion Conjugate() const
	{
		return Quaternion(w, -x, -y, -z);
	}

	T MagnitudeSqrd() const
	{
		return w * w + x * x + y * y + z * z;
	}

	T Magnitude() const
	{
		return Math<T>::Sqrt(w * w + x * x + y * y + z * z);
	}

	Quaternion Normalized() const
	{
		Quaternion result(*this);
		result.Normalize();
		return result;
	}

	Quaternion Normalize()
	{
		T magSqrd = MagnitudeSqrd();

		if (magSqrd > Constant<T>::Zero)
		{
			T invMag = Constant<T>::One / Math<T>::Sqrt(magSqrd);

			w = w * invMag;
			x = x * invMag;
			y = y * invMag;
			z = z * invMag;
		}
	}
	
	T w, x, y, z;
private:
};

typedef Quaternion<float> Quaternionf;