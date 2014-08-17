

#include "OpenGLGraphics.h"

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

GLuint OpenGLTexture2D::GetGLTexture() const
{
	return mTexture;
}

GLenum OpenGLTexture2D::GetTextureType() const
{
	return GL_TEXTURE_2D;
}

void OpenGLTexture2D::LoadMemory(void* source)
{
	GLint existingTexture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &existingTexture);

	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, OpenGLTexture::GetTextureFormat(mFormat), mSize.x, mSize.y, 0, OpenGLTexture::GetTextureChannelType(mFormat.count), OpenGLGraphics::GetGLType(mFormat.type), source);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, existingTexture);
}

void OpenGLTexture2D::GenerateMipmaps()
{
	GLint existingTexture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &existingTexture);

	glBindTexture(GL_TEXTURE_2D, mTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, existingTexture);
}