
#include "IndexBuffer.h"

namespace krono
{

IndexBuffer::IndexBuffer(IndexBuffer::Format format, std::unique_ptr<DataBuffer>& buffer) :
	mFormat(format),
	mBuffer(move(buffer))
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

DataIterator IndexBuffer::Map(size_t indexCount, BufferMapType::Type mappingType)
{
	return mBuffer->Map(indexCount * gFormatSize[mFormat], mappingType);
}

void IndexBuffer::Unmap()
{
	mBuffer->Unmap();
}

void IndexBuffer::Set(const void* data, size_t indexCount)
{
	mBuffer->Set(data, indexCount * gFormatSize[mFormat]);
}

size_t IndexBuffer::GetIndexCount() const
{
	return mBuffer->GetCurrentSize() / gFormatSize[mFormat];
}

DataBuffer& IndexBuffer::GetBuffer()
{
	return *mBuffer;
}

Auto<IndexBuffer> IndexBuffer::Null(NULL);

size_t IndexBuffer::gFormatSize[IndexBuffer::FormatCount] = {sizeof(short), sizeof(long)};

size_t IndexBuffer::GetFormatSize(IndexBuffer::Format format)
{
	return gFormatSize[format];
}

}
