#pragma once

#include "Math/Math.h"

namespace krono
{

template <typename T>
class Color
{
public:
	Color() : r(), g(), b(), a() {}
	Color(const T& r, const T& g, const T& b, const T& a) : r(r), g(g), b(b), a(a) {}
	Color(const T& r, const T& g, const T& b) : r(r), g(g), b(b), a(Constant<T>::One) {}
	~Color(void) {}

	T r, g, b, a;
private:
};

typedef Color<float> Colorf;

template <typename T>
Color<T> operator*(const Color<T>& color, const T& scalar)
{
	return Color<T>(color.r * scalar, color.g * scalar, color.b * scalar, color.a * scalar);
}

}
