
#include "OpenGLTexture.h"

namespace krono
{

OpenGLTexture::OpenGLTexture(void)
{
}


OpenGLTexture::~OpenGLTexture(void)
{
}

GLenum OpenGLTexture::gTextureFormatMapping[DataFormat::TypeCount][4] = {
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
	return gTextureFormatMapping[format.type][format.count - 1];
}

GLenum OpenGLTexture::gTextureChannelMapping[] = {
	GL_RED,
	GL_RG,
	GL_RGB,
	GL_RGBA
};

GLenum OpenGLTexture::GetTextureChannelType(size_t channelCount)
{
	return gTextureChannelMapping[channelCount - 1];
}

}
