#include "stdafx.h"
#include "OpenGLIndexBuffer.h"


OpenGLIndexBuffer::OpenGLIndexBuffer(IndexBuffer::Format format) :
	IndexBuffer(format)
{
	glGenBuffers(1, &mIndexBuffer);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer(void)
{

}

void OpenGLIndexBuffer::Use()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
}

DataIterator OpenGLIndexBuffer::Lock(size_t indexCount)
{
	mBuffer.resize(GetFormatSize(mFormat) * indexCount);
	return DataIterator(&mBuffer.front(), &mBuffer.front() + mBuffer.size());
}

void OpenGLIndexBuffer::Unlock()
{
	GLuint previousIndexBuffer;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&previousIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBuffer.size(), &mBuffer.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, previousIndexBuffer);
}

GLuint OpenGLIndexBuffer::GetIndexBuffer() const
{
	return mIndexBuffer;
}

GLenum OpenGLIndexBuffer::gFormatMapping[] = {
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT
};

GLenum OpenGLIndexBuffer::GetGLFormat(IndexBuffer::Format format)
{
	static_assert(sizeof(gFormatMapping) / sizeof(*gFormatMapping) == IndexBuffer::FormatCount, "gFormatMapping doesn't match IndexBuffer::Format");
	return gFormatMapping[format];
}