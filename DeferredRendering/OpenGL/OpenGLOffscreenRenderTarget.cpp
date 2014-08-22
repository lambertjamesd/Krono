


#include "OpenGLOffscreenRenderTarget.h"

namespace krono
{

OpenGLOffscreenRenderTarget::OpenGLOffscreenRenderTarget(Vector2i size, DataFormat format) :
	RenderTarget(size)
{
	GLuint existingTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&existingTexture);

	glGenTextures(1, &mGLTexture);
	glBindTexture(GL_TEXTURE_2D, mGLTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, OpenGLTexture::GetTextureFormat(format), size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	

	glBindTexture(GL_TEXTURE_2D, existingTexture);

	mTextureTarget = Auto<OpenGLTexture2D>(new OpenGLTexture2D(mGLTexture, size, format));

	GLuint existingBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&existingBuffer);

	glGenFramebuffers(1, &mClearFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mClearFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGLTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, existingBuffer);
}

OpenGLOffscreenRenderTarget::~OpenGLOffscreenRenderTarget(void)
{

}

Auto<Texture2D> OpenGLOffscreenRenderTarget::GetTexture() const
{
	return mTextureTarget;
}

void OpenGLOffscreenRenderTarget::Clear(const Colorf& color)
{
	GLuint existingBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&existingBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, mClearFBO);
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, existingBuffer);
}

OpenGLRenderTarget::Type OpenGLOffscreenRenderTarget::GetType() const
{
	return OpenGLRenderTarget::TypeTexture;
}

GLuint OpenGLOffscreenRenderTarget::GetGLObject() const
{
	return mGLTexture;
}

}
