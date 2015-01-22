#pragma once

#include "DataBuffer.h"
#include "Core/Memory.h"

namespace krono
{

class IndexBuffer
{
public:
	typedef Auto<IndexBuffer> Ptr;

	enum Format
	{
		UInt16,
		UInt32,
		FormatCount
	};

	~IndexBuffer(void);

	Format GetFormat() const;
	DataIterator Map(size_t indexCount, BufferMapType::Type mappingType);
	void Unmap();
	void Set(const void* data, size_t indexCount);

	size_t GetIndexCount() const;

	size_t GetStrideSize() const;
	static size_t GetFormatSize(Format format);

	DataBuffer& GetBuffer();

	static Auto<IndexBuffer> Null;

	IndexBuffer(Format format, std::unique_ptr<DataBuffer>& buffer);
private:
	Format mFormat;
	std::unique_ptr<DataBuffer> mBuffer;

	static size_t gFormatSize[IndexBuffer::FormatCount];
};

}
