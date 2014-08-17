#pragma once

#include "OpenGLCommon.h"

#include "Interface/IndexBuffer.h"

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(IndexBuffer::Format format);
	~OpenGLIndexBuffer(void);

	void Use();
	
	DataIterator Lock(size_t indexCount);
	void Unlock();
	
	virtual size_t GetIndexCount() const;

	GLuint GetIndexBuffer() const;

	static GLenum GetGLFormat(IndexBuffer::Format format);
private:
	static GLenum gFormatMapping[];

	GLuint mIndexBuffer;
	std::vector<char> mBuffer;
	size_t mCurrentIndexBuffer;
};

