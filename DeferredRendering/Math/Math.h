#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

// in order for a custom numerical type ( eg a fixed point number) to work in the math library
// it must implement a template specialization for Constant and Math. It should also
// support the basic math operations such as +-*/% unary -. The type should 
// also support the basic math operations against signed integers

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
	static Type ASin(Type radians);
	static Type Cos(Type radians);
	static Type ACos(Type radians);
	static Type Tan(Type radians);
	static Type ATan(Type radians);
	static Type ATan2(Type y, Type x);

	static Type Abs(Type value);
	static Type Sqrt(Type value);

	static Type Min(Type a, Type b);
	static Type Max(Type a, Type b);

	static Type Sign(Type a);
};

template <>
class Math<float>
{
public:
	static float Sin(float radians)
	{
		return sinf(radians);
	}

	static float ASin(float radians)
	{
		return asinf(radians);
	}
	
	static float Cos(float radians)
	{
		return cosf(radians);
	}
	
	static float ACos(float radians)
	{
		return acosf(radians);
	}

	static float Tan(float radians)
	{
		return tanf(radians);
	}

	static float ATan(float radians)
	{
		return atanf(radians);
	}

	static float ATan2(float y, float x)
	{
		return atan2f(y, x);
	}

	static float Abs(float value)
	{
		return fabs(value);
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

	static float Sign(float a)
	{
		if (a < Constant<float>::Zero)
		{
			return -Constant<float>::One;
		}
		else if (a > Constant<float>::Zero)
		{
			return Constant<float>::One;
		}
		else
		{
			return Constant<float>::Zero;
		}
	}
};

}
