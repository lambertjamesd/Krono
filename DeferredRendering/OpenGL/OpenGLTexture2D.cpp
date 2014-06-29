#include "stdafx.h"
#include "..\stdafx.h"

#include "OpenGLTexture2D.h"

OpenGLTexture2D::OpenGLTexture2D(const Size& size, Format format) :
	Texture2D(size, format)
{
	glGenTextures(1, &mTexture);
	Resize(size);
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

void OpenGLTexture2D::BindToSlot(int slotIndex)
{

}

void OpenGLTexture2D::Resize(const Size& size)
{
	mSize = size;
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, glFormatMap[mFormat], mSize.x, mSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

GLuint OpenGLTexture2D::GetGLTexture() const
{
	return mTexture;
}