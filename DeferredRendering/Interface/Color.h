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
typedef Color<unsigned char> ColorU8;

template <typename T>
Color<T> operator*(const Color<T>& color, const T& scalar)
{
	return Color<T>(color.r * scalar, color.g * scalar, color.b * scalar, color.a * scalar);
}

Colorf ColorU82F(const ColorU8& byteColor);
ColorU8 ColorF2U8(const Colorf& byteColor);

}
