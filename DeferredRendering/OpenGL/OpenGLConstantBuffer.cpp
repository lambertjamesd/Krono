

#include "OpenGLConstantBuffer.h"

using namespace std;

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
	const vector<pair<ConstantBufferLayout::Type, size_t> >& typeLocations = mLayout.GetSpecialTypes();
	char* dataPtr = &mBufferCopy.front();

	for (auto it = typeLocations.cbegin(); it != typeLocations.cend(); ++it)
	{
		switch (it->first)
		{
		case ConstantBufferLayout::TypeProjectionMatrix:
			{
				Matrix4f *matrixPtr = (Matrix4f*)(dataPtr + it->second);
				*matrixPtr = gProjectionMatrixConversion * *matrixPtr;
				break;
			}
		case ConstantBufferLayout::TypeInvProjectionMatrix:
			{
				Matrix4f *matrixPtr = (Matrix4f*)(dataPtr + it->second);
				*matrixPtr = *matrixPtr * gInvProjectionMatrixConversion;
				break;
			}
		default:
			break;
		}
	}
}


Matrix4f OpenGLConstantBuffer::gProjectionMatrixConversion(Matrix4f::Translation(Vector3f(0.0f, 0.0f, -1.0f)) * Matrix4f::Scale(Vector3f(1.0f, 1.0f, 2.0f)));
Matrix4f OpenGLConstantBuffer::gInvProjectionMatrixConversion(Matrix4f::Scale(Vector3f(1.0f, 1.0f, 0.5f)) * Matrix4f::Translation(Vector3f(0.0f, 0.0f, 1.0f)));