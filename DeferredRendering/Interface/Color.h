#pragma once

namespace krono
{

template <typename T>
class Color
{
public:
	Color(const T& r, const T& g, const T& b, const T& a) : r(r), g(g), b(b), a(a) {};
	~Color(void) {};

	T r, g, b, a;
private:
};

typedef Color<float> Colorf;

}
