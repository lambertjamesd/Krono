
#include "OpenGLIndexBuffer.h"

namespace krono
{

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
	mCurrentIndexBuffer = indexCount;
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

size_t OpenGLIndexBuffer::GetIndexCount() const
{
	return mCurrentIndexBuffer;
}

GLuint OpenGLIndexBuffer::GetIndexBuffer() const
{
	return mIndexBuffer;
}

GLenum OpenGLIndexBuffer::gFormatMapping[IndexBuffer::FormatCount] = {
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT
};

GLenum OpenGLIndexBuffer::GetGLFormat(IndexBuffer::Format format)
{
	return gFormatMapping[format];
}

}
