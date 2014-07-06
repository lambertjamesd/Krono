#pragma once

#include "..\Interface\IndexBuffer.h"

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(IndexBuffer::Format format);
	~OpenGLIndexBuffer(void);

	void Use();
	
	DataIterator Lock(size_t indexCount);
	void Unlock();

	GLuint GetIndexBuffer() const;

	static GLenum GetGLFormat(IndexBuffer::Format format);
private:
	GLuint mIndexBuffer;
	std::vector<char> mBuffer;

	static GLenum gFormatMapping[];
};

