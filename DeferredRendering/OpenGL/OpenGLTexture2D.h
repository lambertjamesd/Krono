#pragma once
#include "..\Interface\Texture2D.h"

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(const Size& size, DataFormat format);
	virtual ~OpenGLTexture2D();

	GLuint GetGLTexture() const;
protected:
	GLuint mTexture;

	static GLint glFormatMap[];
};