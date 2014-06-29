#pragma once

#include "..\Interface\VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(const InputLayout& inputLayout);
	virtual ~OpenGLVertexBuffer();
	
	virtual VertexBufferIterator Lock(size_t vertexCount);
	virtual void Unlock();
	
	virtual void Use();
private:

	static GLenum GLTypeMapping[]; 

	std::vector<char> mBuffer;
	GLuint mVertexBuffer;
};