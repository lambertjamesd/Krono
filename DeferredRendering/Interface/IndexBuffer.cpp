
#include "IndexBuffer.h"

namespace krono
{

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

Auto<IndexBuffer> IndexBuffer::Null(NULL);

size_t IndexBuffer::gFormatSize[IndexBuffer::FormatCount] = {sizeof(short), sizeof(long)};

size_t IndexBuffer::GetFormatSize(IndexBuffer::Format format)
{
	return gFormatSize[format];
}

}
