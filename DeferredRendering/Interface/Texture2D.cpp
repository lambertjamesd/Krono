
#include "Texture2D.h"


namespace krono
{

Texture2D::Texture2D(const Vector2i& size, DataFormat format) :
	mSize(size),
	mFormat(format)
{

}

Texture2D::~Texture2D(void)
{

}

Vector2i Texture2D::GetSize() const
{
	return mSize;
}

DataFormat Texture2D::GetFormat() const
{
	return mFormat;
}

const Texture2D::Ptr Texture2D::Null = NULL;

}
