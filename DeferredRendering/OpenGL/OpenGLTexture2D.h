#pragma once
#include "Interface/Texture2D.h"
#include "OpenGLTexture.h"

class OpenGLTexture2D : public Texture2D, public OpenGLTexture
{
public:
	OpenGLTexture2D(GLuint texture, const Vector2i& size, DataFormat format);
	OpenGLTexture2D(const Vector2i& size, DataFormat format);
	virtual ~OpenGLTexture2D();
	
	virtual GLenum GetTextureType() const;
	virtual GLuint GetGLTexture() const;
	
	virtual void LoadMemory(void* source);

	virtual void GenerateMipmaps();
protected:
	GLuint mTexture;
};