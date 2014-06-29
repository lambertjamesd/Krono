#include "stdafx.h"
#include "Texture2D.h"

Texture2D::Texture2D(const Size& size, Format format) :
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

Texture2D::Format Texture2D::GetFormat() const
{
	return mFormat;
}