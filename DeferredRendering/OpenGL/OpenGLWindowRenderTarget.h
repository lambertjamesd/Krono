#pragma once

#include "..\Interface\Window.h"
#include "../Interface/RenderTarget.h"

#ifdef _WIN32

#include <wingdi.h>

class OpenGLWindowRenderTarget : public WindowRenderTarget
{
public:
	OpenGLWindowRenderTarget(Window& window);
	~OpenGLWindowRenderTarget(void);
	
	virtual void GetRenderTargetInternal(void* target) const;
	virtual Auto<Texture2D> GetTexture() const;
	virtual void Clear(const Colorf& color);
	
	virtual void Present(void);
private:

	void MakeActive();

	HGLRC mHGLRenderContext;
	HDC mHDC;
};

#endif