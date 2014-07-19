#pragma once

#include "..\Interface\ConstantBuffer.h"
#include "..\Math\Matrix.h"

class OpenGLConstantBuffer : public ConstantBuffer
{
public:
	OpenGLConstantBuffer(const ConstantBufferLayout& layout);
	~OpenGLConstantBuffer(void);
	
	virtual void Set(const void* data, size_t size);

	GLuint GetBuffer();
private:
	void ModifyProjectionMatrices();

	static Matrix4f gProjectionMatrixConversion;

	std::vector<char> mBufferCopy;
	GLuint mBuffer;
};
