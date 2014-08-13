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
	{GL_R8, GL_RG8, GL_RGB8, GL_RGBA8},
	{GL_DEPTH_COMPONENT16, 0, 0, 0},
	{GL_DEPTH_COMPONENT24, 0, 0, 0},
	{GL_DEPTH_COMPONENT32F, 0, 0, 0},
	{GL_DEPTH24_STENCIL8, 0, 0, 0},
	{GL_DEPTH32F_STENCIL8, 0, 0, 0},
};

GLenum OpenGLTexture::GetTextureFormat(DataFormat format)
{
	static_assert(sizeof(gTextureFormatMapping) / sizeof(*gTextureFormatMapping) == DataFormat::TypeCount, "gTextureFormatMapping doesn't match");
	return gTextureFormatMapping[format.type][format.count - 1];
}

GLenum OpenGLTexture::gTextureChannelMapping[] = {
	GL_R,
	GL_RG,
	GL_RGB,
	GL_RGBA
};

GLenum OpenGLTexture::GetTextureChannelType(size_t channelCount)
{
	return gTextureChannelMapping[channelCount - 1];
}