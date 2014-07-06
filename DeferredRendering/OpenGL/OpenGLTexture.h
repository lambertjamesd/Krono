#pragma once

#include "..\Interface\DataFormat.h"

class OpenGLTexture
{
public:
	virtual GLenum GetTextureType() const = 0;
	virtual GLuint GetGLTexture() const = 0;

	static GLenum GetTextureFormat(DataFormat format);
protected:
	OpenGLTexture(void);
	~OpenGLTexture(void);

	static GLenum gTextureFormatMapping[][4];
};

