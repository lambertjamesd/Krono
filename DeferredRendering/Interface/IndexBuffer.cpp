#include "stdafx.h"
#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(IndexBuffer::Format format) :
	mFormat(format)
{
}

IndexBuffer::~IndexBuffer(void)
{
}

size_t IndexBuffer::GetStrideSize() const
{
	return gFormatSize[mFormat];
}

IndexBuffer::Format IndexBuffer::GetFormat() const
{
	return mFormat;
}

size_t IndexBuffer::gFormatSize[] = {sizeof(short), sizeof(long)};

size_t IndexBuffer::GetFormatSize(IndexBuffer::Format format)
{
	static_assert(sizeof(gFormatSize) / sizeof(*gFormatSize) == IndexBuffer::FormatCount, "Missing elements in gFormatSize");
	return gFormatSize[format];
}