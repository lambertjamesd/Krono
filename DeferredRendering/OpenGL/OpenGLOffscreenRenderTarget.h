#pragma once

#include "Interface\RenderTarget.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLTexture2D.h"

class OpenGLOffscreenRenderTarget : public RenderTarget, public OpenGLRenderTarget
{
public:
	OpenGLOffscreenRenderTarget(Vector2i size, DataFormat format);
	~OpenGLOffscreenRenderTarget(void);
	
	virtual Auto<Texture2D> GetTexture() const;
	virtual void Clear(const Colorf& color);
	
	virtual OpenGLRenderTarget::Type GetType() const;
	virtual GLuint GetGLObject() const;
private:
	GLuint mClearFBO;
	GLuint mGLTexture;
	Auto<OpenGLTexture2D> mTextureTarget;
};

