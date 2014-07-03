#pragma once

#include "VertexBuffer.h"

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

	size_t GetStrideSize() const;
	static size_t GetFormatSize(Format format);
protected:
	IndexBuffer(Format format);
private:
	Format mFormat;

	static size_t gFormatSize[];
};

