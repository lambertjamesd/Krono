#pragma once
#include "..\Interface\Texture2D.h"

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(const Size& size, Format format);
	virtual ~OpenGLTexture2D();

	virtual void BindToSlot(int slotIndex);
	virtual void Resize(const Size& size);

	GLuint GetGLTexture() const;
protected:
	GLuint mTexture;

	static GLint glFormatMap[];
};