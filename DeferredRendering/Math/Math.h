#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

template <typename Type>
class Constant
{
public:
	static const Type Zero;
	static const Type One;
	
	static const Type PI;
	
	static const Type E;

	static const Type Deg2Rad;
	static const Type Rad2Deg;
};


////////////////////////////////
// float
template <>
const float Constant<float>::Zero = 0.0f;
template <>
const float Constant<float>::One = 1.0f;
template <>
const float Constant<float>::PI = (float)(M_PI);
template <>
const float Constant<float>::E = (float)(M_E);
template <>
const float Constant<float>::Deg2Rad = (float)(M_PI / 180.0);
template <>
const float Constant<float>::Rad2Deg = (float)(180.0 / M_PI);

////////////////////////////////
// double
template <>
const double Constant<double>::Zero = 0.0;
template <>
const double Constant<double>::One = 1.0;
template <>
const double Constant<double>::PI = M_PI;
template <>
const double Constant<double>::E = M_E;
template <>
const double Constant<double>::Deg2Rad = M_PI / 180.0;
template <>
const double Constant<double>::Rad2Deg = 180.0 / M_PI;

////////////////////////////////
// int
template <>
const int Constant<int>::Zero = 0;
template <>
const int Constant<int>::One = 1;


template <typename Type>
class Math
{
public:
	static Type Sin(Type radians);
	static Type Cos(Type radians);
	static Type Tan(Type radians);
	static Type Sqrt(Type radians);
};

template <>
class Math<float>
{
public:
	static float Sin(float radians)
	{
		return sinf(radians);
	}
	
	static float Cos(float radians)
	{
		return cosf(radians);
	}

	static float Tan(float radians)
	{
		return tanf(radians);
	}

	static float Sqrt(float value)
	{
		return sqrtf(value);
	}
};