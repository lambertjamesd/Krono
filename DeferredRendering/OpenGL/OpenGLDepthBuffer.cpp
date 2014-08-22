
#include "OpenGLDepthBuffer.h"
#include <iostream>

namespace krono
{

OpenGLDepthBuffer::OpenGLDepthBuffer(Vector2i size, DataFormat::Type format) :
	DepthBuffer(size, format)
{
	DataFormat dataFormat(format, 1);

	GLuint existingTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&existingTexture);

	glGenTextures(1, &mGLTexture);
	glBindTexture(GL_TEXTURE_2D, mGLTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, OpenGLTexture2D::GetTextureFormat(dataFormat), size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, existingTexture);

	mTextureTarget = Auto<OpenGLTexture2D>(new OpenGLTexture2D(mGLTexture, size, dataFormat));
	
	GLuint existingBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&existingBuffer);

	glGenFramebuffers(1, &mClearFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mClearFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mGLTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, existingBuffer);
}

OpenGLDepthBuffer::~OpenGLDepthBuffer(void)
{

}

Auto<Texture2D> OpenGLDepthBuffer::GetTexture() const
{
	return mTextureTarget;
}

void OpenGLDepthBuffer::Clear(float depth)
{
	GLuint existingBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&existingBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, mClearFBO);
	glClearDepthf(depth);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, existingBuffer);
}

OpenGLRenderTarget::Type OpenGLDepthBuffer::GetType() const
{
	return OpenGLRenderTarget::TypeTexture;
}

GLuint OpenGLDepthBuffer::GetGLObject() const
{
	return mGLTexture;
}

}
