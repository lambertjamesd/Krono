#pragma once
#include "Vector2.h"

namespace krono
{

template <typename T>
class Rect
{
public:
	Rect(const Vector2<T>& topLeft, const Vector2<T>& size) : topLeft(topLeft), size(size) {};
	Rect(void) {};
	~Rect(void) {};
	
	Vector2<T> topLeft;
	Vector2<T> size;
private:
};

typedef Rect<int> Recti;
typedef Rect<float> Rectf;

}
