#pragma once

#include "Math.h"
#include "Vector3.h"
#include "Angle.h"

namespace krono
{

template <typename T>
class Quaternion
{
public:
	Quaternion() : w(Constant<T>::One), x(T()), y(T()), z(T()) {}
	Quaternion(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {}
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

	Quaternion(const Vector3<T>& right, const Vector3<T>& up, const Vector3<T>& look)
	{ 
		T trace = right.x + up.y + look.z;
		if( trace > Constant<T>::Zero )
		{
			T s = Constant<T>::One / (sqrtf(trace + Constant<T>::One) * 2);
			w = Constant<T>::One / (s * 4);
			x = ( up.z - look.y ) * s;
			y = ( look.x - right.z ) * s;
			z = ( right.y - up.x ) * s;
		}
		else if ( right.x > up.y && right.x > look.z )
		{
			T s = 2 * Math<T>::Sqrt( 1 + right.x - up.y - look.z);
			w = (up.z - look.y ) / s;
			x = s / 4;
			y = (up.x + right.y ) / s;
			z = (look.x + right.z ) / s;
		}
		else if (up.y > look.z)
		{
			T s = 2 * sqrtf( 1 + up.y - right.x - look.z);
			w = (look.x - right.z ) / s;
			x = (up.x + right.y ) / s;
			y = s / 4;
			z = (look.y + up.z ) / s;
		}
		else
		{
			T s = 2 * sqrtf( 1 + look.z - right.x - up.y );
			w = (right.y - up.x ) / s;
			x = (look.x + right.z ) / s;
			y = (look.y + up.z ) / s;
			z = s / 4;
		}
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

	void ToAxisAngle(Vector3<T>& axis, Radians<T>& angle) const
	{
		axis = Vector3<T>(x, y, z).Normalized();

		T positiveW = w;
		
		if (positiveW < Constant<T>::Zero)
		{
			axis = axis * -Constant<T>::One;
			positiveW = positiveW * -Constant<T>::One;
		}

		if (positiveW > Constant<T>::One)
		{
			positiveW = Constant<T>::One;
		}

		angle = Radians<T>(Math<T>::ACos(positiveW * 2));
	}

	static Quaternion Identity()
	{
		return Quaternion();
	}
	
	T w, x, y, z;
private:
};

typedef Quaternion<float> Quaternionf;

}
