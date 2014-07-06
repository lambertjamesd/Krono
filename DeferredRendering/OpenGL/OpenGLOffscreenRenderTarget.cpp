#include "stdafx.h"
#include "..\stdafx.h"

#include "OpenGLOffscreenRenderTarget.h"

OpenGLOffscreenRenderTarget::OpenGLOffscreenRenderTarget(Vector2i size, DataFormat format)
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

}

OpenGLRenderTarget::Type OpenGLOffscreenRenderTarget::GetType() const
{
	return OpenGLRenderTarget::TypeTexture;
}

GLuint OpenGLOffscreenRenderTarget::GetGLObject() const
{
	return mGLTexture;
}