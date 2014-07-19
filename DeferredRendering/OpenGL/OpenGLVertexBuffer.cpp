#include "stdafx.h"
#include "OpenGLVertexBuffer.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const InputLayout& inputLayout) :
	VertexBuffer(inputLayout),
	mCurrentVertexCount(0)
{
	glGenBuffers(1, &mVertexBuffer);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{

}
	
DataIterator OpenGLVertexBuffer::Lock(size_t vertexCount)
{
	FlipYUV();

	mCurrentVertexCount = vertexCount;
	size_t bufferSize = vertexCount * mInputLayout.GetStride();
	mBuffer.resize(bufferSize);
	char* rawPointer = &mBuffer.front();

	return DataIterator(rawPointer, rawPointer + bufferSize);
}

void OpenGLVertexBuffer::Unlock()
{
	FlipYUV();

	GLuint previousArrayBuffer;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&previousArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mBuffer.size(), &mBuffer.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, previousArrayBuffer);
}

size_t OpenGLVertexBuffer::GetVertexCount() const
{
	return mCurrentVertexCount;
}

void OpenGLVertexBuffer::Use()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
}

void OpenGLVertexBuffer::FlipYUV()
{
	if (mCurrentVertexCount > 0)
	{
		size_t offset = 0;

		for (size_t i = 0; i < mInputLayout.GetAttributeCount(); ++i)
		{
			const Attribute& attrib = mInputLayout.GetAttribute(i);

			if (attrib.GetName() == "TEXCOORD")
			{
				switch (attrib.GetFormat().type)
				{
				case DataFormat::Float:
					FlipValue<float, 1>(offset + sizeof(float), mInputLayout.GetStride());
					break;
				case DataFormat::Int8UNorm:
					FlipValue<unsigned char, 255>(offset + sizeof(char), mInputLayout.GetStride());
					break;
				}
			}

			offset += attrib.GetFormat().GetSize();
		}
	}
}