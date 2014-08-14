
#include "DepthBuffer.h"


DepthBuffer::DepthBuffer(Vector2i size, DataFormat::Type format) :
	mSize(size),
	mFormat(format)
{

}


DepthBuffer::~DepthBuffer(void)
{

}

DataFormat::Type DepthBuffer::GetFormat() const
{
	return mFormat;
}

Vector2i DepthBuffer::GetSize() const
{
	return mSize;
}

Auto<DepthBuffer> DepthBuffer::Null(NULL);