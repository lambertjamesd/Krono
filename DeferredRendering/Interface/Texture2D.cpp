#include "stdafx.h"
#include "Texture2D.h"

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