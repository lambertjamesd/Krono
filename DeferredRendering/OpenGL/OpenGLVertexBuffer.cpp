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
	
DataIterator OpenGLVertexBuffer::Lock(size_t vertexCount)
{
	size_t bufferSize = vertexCount * mInputLayout.GetStride();
	mBuffer.resize(bufferSize);
	char* rawPointer = &mBuffer.front();

	return DataIterator(rawPointer, rawPointer + bufferSize);
}

void OpenGLVertexBuffer::Unlock()
{
	GLuint previousArrayBuffer;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&previousArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mBuffer.size(), &mBuffer.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, previousArrayBuffer);
}

void OpenGLVertexBuffer::Use()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
}

GLenum OpenGLVertexBuffer::GLTypeMapping[] = {GL_FLOAT, GL_UNSIGNED_BYTE};