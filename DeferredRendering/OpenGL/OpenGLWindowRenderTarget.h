#pragma once

#include "OpenGLCommon.h"

#include "Interface/Window.h"
#include "Interface/RenderTarget.h"
#include "OpenGLRenderTarget.h"

#if defined(_WIN32)

#include <wingdi.h>

#elif defined(USE_XLIB)

#include <GL/glx.h>

#endif


namespace krono
{

#if defined(_WIN32)

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

#elif defined(USE_XLIB)

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

	GLXContext mContext;
	WindowHandle mWindowHandle;
	Display *mWindowDisplay;
};

#else

#error No window library specified

#endif

}
