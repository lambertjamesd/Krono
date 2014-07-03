#pragma once

#include "..\Interface\VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(const InputLayout& inputLayout);
	virtual ~OpenGLVertexBuffer();
	
	virtual DataIterator Lock(size_t vertexCount);
	virtual void Unlock();
	
	void Use();
private:

	static GLenum GLTypeMapping[]; 

	std::vector<char> mBuffer;
	GLuint mVertexBuffer;
};