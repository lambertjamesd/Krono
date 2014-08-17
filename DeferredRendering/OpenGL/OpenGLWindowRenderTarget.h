#pragma once

#include "OpenGLCommon.h"

#include "Interface/Window.h"
#include "Interface/RenderTarget.h"
#include "OpenGLRenderTarget.h"

#ifdef _WIN32

#include <wingdi.h>

class OpenGLWindowRenderTarget : public WindowRenderTarget, public OpenGLRenderTarget
{
public:
	OpenGLWindowRenderTarget(Window& window);
	~OpenGLWindowRenderTarget(void);

	virtual Auto<Texture2D> GetTexture() const;
	virtual void Clear(const Colorf& color);
	
	virtual void Present(void);
	
	virtual OpenGLRenderTarget::Type GetType() const;
	virtual GLuint GetGLObject() const;

private:

	void MakeActive();

	HGLRC mHGLRenderContext;
	HDC mHDC;
};

#endif