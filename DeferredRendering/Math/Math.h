#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace krono
{

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


template <typename Type>
class Math
{
public:
	static Type Sin(Type radians);
	static Type Cos(Type radians);
	static Type Tan(Type radians);
	static Type Sqrt(Type radians);

	static Type Min(Type a, Type b);
	static Type Max(Type a, Type b);
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

	static float Min(float a, float b)
	{
		return a < b ? a : b;
	}

	static float Max(float a, float b)
	{
		return a < b ? b : a;
	}
};

}
