#include "stdafx.h"
#include "OpenGLVertexBuffer.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const InputLayout& inputLayout) :
	VertexBuffer(inputLayout)
{
	glGenBuffers(1, &mVertexBuffer);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{

}
	
VertexBufferIterator OpenGLVertexBuffer::Lock(size_t vertexCount)
{
	size_t bufferSize = vertexCount * mInputLayout.GetStride();
	mBuffer.resize(bufferSize);
	char* rawPointer = &mBuffer.front();

	return VertexBufferIterator(rawPointer, rawPointer + bufferSize);
}

void OpenGLVertexBuffer::Unlock()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mBuffer.size(), &mBuffer.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::Use()
{

}

GLenum OpenGLVertexBuffer::GLTypeMapping[] = {GL_FLOAT, GL_UNSIGNED_BYTE};