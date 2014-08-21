#pragma once

#include "VertexBuffer.h"
#include "Core/Memory.h"

class IndexBuffer
{
public:
	enum Format
	{
		UInt16,
		UInt32,
		FormatCount
	};

	~IndexBuffer(void);

	Format GetFormat() const;
	virtual DataIterator Lock(size_t indexCount) = 0;
	virtual void Unlock() = 0;

	virtual size_t GetIndexCount() const = 0;

	size_t GetStrideSize() const;
	static size_t GetFormatSize(Format format);

	static Auto<IndexBuffer> Null;
protected:
	IndexBuffer(Format format);
	
	Format mFormat;
private:

	static size_t gFormatSize[IndexBuffer::FormatCount];
};

