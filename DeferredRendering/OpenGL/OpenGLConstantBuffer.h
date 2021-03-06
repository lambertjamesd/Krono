#pragma once

#include "OpenGLCommon.h"

#include "Interface/ConstantBuffer.h"
#include "Math/Matrix.h"

namespace krono
{

class OpenGLConstantBuffer : public ConstantBuffer
{
public:
	typedef Auto<OpenGLConstantBuffer> Ptr;

	OpenGLConstantBuffer(const ConstantBufferLayout& layout);
	~OpenGLConstantBuffer(void);
	
	virtual void Set(const void* data, size_t size);

	GLuint GetBuffer() const;
private:
	void ModifyProjectionMatrices();

	static Matrix4f gProjectionMatrixConversion;
	static Matrix4f gInvProjectionMatrixConversion;

	std::vector<char> mBufferCopy;
	GLuint mBuffer;
};

}
