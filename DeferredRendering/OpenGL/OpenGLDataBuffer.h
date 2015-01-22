#pragma once

#include "OpenGLCommon.h"
#include "Interface/DataBuffer.h"

namespace krono
{

class OpenGLDataBuffer :
	public DataBuffer
{
public:
	OpenGLDataBuffer(BufferType type, BufferAccess::Type bufferAccess);
	~OpenGLDataBuffer(void);

	virtual DataIterator Map(size_t vertexCount, BufferMapType::Type mappingType);
	virtual void Unmap();
	virtual void Set(const void* data, size_t size);
	
	GLuint GetIndexBuffer() const;
	void Use();
private:
	GLuint mBuffer;

	static GLenum gBufferBindingTypes[DataBuffer::TypeCount];
	static GLenum gBufferTypes[DataBuffer::TypeCount];
	static GLenum gAccessMapping[BufferAccess::Count];
	
	static GLenum gMapAccessMapping[BufferMapType::Count];
};

}