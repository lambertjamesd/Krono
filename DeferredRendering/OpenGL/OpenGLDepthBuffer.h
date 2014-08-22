#pragma once

#include "Interface/DepthBuffer.h"
#include "OpenGLObjectID.h"
#include "OpenGLTexture2D.h"
#include "OpenGLRenderTarget.h"

namespace krono
{

class OpenGLDepthBuffer : public DepthBuffer, public OpenGLRenderTarget
{
public:
	OpenGLDepthBuffer(Vector2i size, DataFormat::Type format);
	~OpenGLDepthBuffer(void);
	
	virtual Auto<Texture2D> GetTexture() const;
	virtual void Clear(float depth);
	
	virtual OpenGLRenderTarget::Type GetType() const;
	virtual GLuint GetGLObject() const;
private:
	GLuint mClearFBO;
	GLuint mGLTexture;
	Auto<OpenGLTexture2D> mTextureTarget;

	static GLenum gTextureFormatMapping[];
};

}
