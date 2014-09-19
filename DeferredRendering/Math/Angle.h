#pragma once

#include "Math.h"

namespace krono
{

template <typename T>
class Degrees;

template <typename T>
class Radians
{
public:
	Radians(const Degrees<T>& degrees) : mValue(degrees.mValue * Constant<T>::Deg2Rad) {}
	Radians() : mValue(Constant<T>::Zero) {}
	explicit Radians(const T& value) : mValue(value) {}

	operator T() const
	{
		return mValue;
	}
private:
	friend class Degrees<T>;

	T mValue;
};

template <typename T>
class Degrees
{
public:
	Degrees(const Radians<T>& radians) : mValue(radians.mValue * Constant<T>::Rad2Deg) {}
	Degrees() : mValue(Constant<T>::Zero) {}
	explicit Degrees(const T& value) : mValue(value) {}

	operator T() const
	{
		return mValue;
	}
private:
	friend class Radians<T>;

	T mValue;
};

typedef Degrees<float> Degreesf;
typedef Radians<float> Radiansf;

template <typename T>
Radians<T> operator*(const Radians<T>& a, const Radians<T>& b)
{
	return Radians<T>((T)a * (T)b);
}

template <typename T>
Radians<T> operator*(const Radians<T>& a, const T& b)
{
	return Radians<T>((T)a * b);
}

template <typename T>
Radians<T> operator*(const T& a, const Radians<T>& b)
{
	return Radians<T>(a * (T)b);
}

}
