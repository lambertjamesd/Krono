#include "stdafx.h"
#include "..\stdafx.h"

#include "OpenGLTexture2D.h"

OpenGLTexture2D::OpenGLTexture2D(GLuint texture, const Vector2i& size, DataFormat format) :
	Texture2D(size, format),
	mTexture(texture)
{

}

OpenGLTexture2D::OpenGLTexture2D(const Vector2i& size, DataFormat format) :
	Texture2D(size, format)
{
	glGenTextures(1, &mTexture);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &mTexture);
}

GLint OpenGLTexture2D::glFormatMap[] = {
	GL_RGB8UI,
	GL_RGB16F,
	GL_RGB32F,

	GL_RGBA8UI,
	GL_RGBA16F,
	GL_RGBA32F
};

GLuint OpenGLTexture2D::GetGLTexture() const
{
	return mTexture;
}

GLenum OpenGLTexture2D::GetTextureType() const
{
	return GL_TEXTURE_2D;
}