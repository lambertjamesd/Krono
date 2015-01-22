#include "OpenGLDataBuffer.h"

namespace krono
{

OpenGLDataBuffer::OpenGLDataBuffer(BufferType type, BufferAccess::Type bufferAccess) :
	DataBuffer(type, bufferAccess)
{
	glGenBuffers(1, &mBuffer);
}


OpenGLDataBuffer::~OpenGLDataBuffer(void)
{

}

DataIterator OpenGLDataBuffer::Map(size_t size, BufferMapType::Type mappingType)
{
	GLuint previousArrayBuffer;
	glGetIntegerv(gBufferBindingTypes[mBufferType], (GLint*)&previousArrayBuffer);
	glBindBuffer(gBufferTypes[mBufferType], mBuffer);
	
	if (mCurrentSize != size)
	{
		glBufferData(gBufferTypes[mBufferType], size, nullptr, gAccessMapping[mBufferAccess]);
		mCurrentSize = size;
	}

	void* result = glMapBufferRange(gBufferTypes[mBufferType], 0, size, gMapAccessMapping[mappingType]);
	glBindBuffer(gBufferTypes[mBufferType], previousArrayBuffer);

	return DataIterator(result, (void*)((char*)result + size));
}

void OpenGLDataBuffer::Unmap()
{
	GLuint previousArrayBuffer;
	glGetIntegerv(gBufferBindingTypes[mBufferType], (GLint*)&previousArrayBuffer);
	glBindBuffer(gBufferTypes[mBufferType], mBuffer);
	glUnmapBuffer(gBufferTypes[mBufferType]);
	glBindBuffer(gBufferTypes[mBufferType], previousArrayBuffer);
}

void OpenGLDataBuffer::Set(const void* data, size_t size)
{
	GLuint previousArrayBuffer;
	glGetIntegerv(gBufferBindingTypes[mBufferType], (GLint*)&previousArrayBuffer);
	glBindBuffer(gBufferTypes[mBufferType], mBuffer);
	glBufferData(gBufferTypes[mBufferType], size, data, gAccessMapping[mBufferAccess]);
	glBindBuffer(gBufferTypes[mBufferType], previousArrayBuffer);
	mCurrentSize = size;
}

GLuint OpenGLDataBuffer::GetIndexBuffer() const
{
	return mBuffer;
}

void OpenGLDataBuffer::Use()
{
	glBindBuffer(gBufferTypes[mBufferType], mBuffer);
}

GLenum OpenGLDataBuffer::gBufferBindingTypes[BufferType::TypeCount] = {
	GL_ARRAY_BUFFER_BINDING,
	GL_ELEMENT_ARRAY_BUFFER_BINDING,
	GL_UNIFORM_BUFFER_BINDING
};

GLenum OpenGLDataBuffer::gBufferTypes[BufferType::TypeCount] = {
	GL_ARRAY_BUFFER,
	GL_ELEMENT_ARRAY_BUFFER,
	GL_UNIFORM_BUFFER
};

GLenum OpenGLDataBuffer::gAccessMapping[BufferAccess::Count] = {
	GL_STREAM_COPY,
	GL_STATIC_DRAW,
	GL_DYNAMIC_DRAW,
	GL_STREAM_COPY,
};

GLenum OpenGLDataBuffer::gMapAccessMapping[BufferMapType::Count] = {
	GL_MAP_READ_BIT,
	GL_MAP_WRITE_BIT,
	GL_MAP_READ_BIT | GL_MAP_WRITE_BIT,
	GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT,
	GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT
};

}