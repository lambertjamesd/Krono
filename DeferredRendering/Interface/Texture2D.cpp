#include "stdafx.h"
#include "Texture2D.h"

Texture2D::Texture2D(const Size& size, DataFormat format) :
	mSize(size),
	mFormat(format)
{

}

Texture2D::~Texture2D(void)
{

}

Size Texture2D::GetSize() const
{
	return mSize;
}

DataFormat Texture2D::GetFormat() const
{
	return mFormat;
}