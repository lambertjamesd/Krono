#pragma once

#include "OpenGLCommon.h"

#include "OpenGLCommon.h"

#include "Interface/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(const InputLayout& inputLayout);
	virtual ~OpenGLVertexBuffer();
	
	virtual DataIterator Lock(size_t vertexCount);
	virtual void Unlock();
	
	virtual size_t GetVertexCount() const;
	
	void Use();
private:
	template <typename T, int one>
	void FlipValue(size_t offset, size_t stride)
	{
		char* rawPointer = &mBuffer.front() + offset;

		for (size_t i = 0; i < mCurrentVertexCount; ++i)
		{
			*((T*)rawPointer) = one - *((T*)rawPointer);
			rawPointer += stride;
		}
	}

	void FlipYUV();

	std::vector<char> mBuffer;
	GLuint mVertexBuffer;
	size_t mCurrentVertexCount;
};