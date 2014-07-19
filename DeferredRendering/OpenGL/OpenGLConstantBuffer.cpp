#include "stdafx.h"
#include "OpenGLConstantBuffer.h"

OpenGLConstantBuffer::OpenGLConstantBuffer(const ConstantBufferLayout& layout) :
	ConstantBuffer(layout)
{
	glGenBuffers(1, &mBuffer);
}


OpenGLConstantBuffer::~OpenGLConstantBuffer(void)
{

}

void OpenGLConstantBuffer::Set(const void* data, size_t size)
{
	mBufferCopy.resize(size);
	memcpy(&mBufferCopy.front(), data, size);
	ModifyProjectionMatrices();

	GLuint existingBuffer;
	glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, (GLint*)&existingBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
	glBufferData(GL_UNIFORM_BUFFER, size, &mBufferCopy.front(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, existingBuffer);
}

GLuint OpenGLConstantBuffer::GetBuffer()
{
	return mBuffer;
}

void OpenGLConstantBuffer::ModifyProjectionMatrices()
{
	const std::vector<size_t>& projectionLocations = mLayout.GetProjectionMatrixPositions();
	char* dataPtr = &mBufferCopy.front();

	for (auto it = projectionLocations.cbegin(); it != projectionLocations.cend(); ++it)
	{
		Matrix4f *matrixPtr = (Matrix4f*)(dataPtr + *it);

		*matrixPtr = gProjectionMatrixConversion * *matrixPtr;
	}
}


Matrix4f OpenGLConstantBuffer::gProjectionMatrixConversion(ScaleMatrix(Vector3f(-1.0f, 1.0f, 2.0f)) * TranslationMatrix(Vector3f(0.0f, 0.0f, -0.5f)));