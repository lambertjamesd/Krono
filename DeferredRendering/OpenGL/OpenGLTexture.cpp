#include "stdafx.h"
#include "OpenGLTexture.h"


OpenGLTexture::OpenGLTexture(void)
{
}


OpenGLTexture::~OpenGLTexture(void)
{
}

GLenum OpenGLTexture::gTextureFormatMapping[][4] = {
	{GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F},
	{GL_R8UI, GL_RG8UI, GL_RGB8UI, GL_RGBA8UI},
};

GLenum OpenGLTexture::GetTextureFormat(DataFormat format)
{
	static_assert(sizeof(gTextureFormatMapping) / sizeof(*gTextureFormatMapping) == DataFormat::TypeCount, "gTextureFormatMapping doesn't match");
	return gTextureFormatMapping[format.type][format.count - 1];
}
