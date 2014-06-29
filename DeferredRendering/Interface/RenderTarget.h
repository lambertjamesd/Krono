#pragma once
#include "Color.h"

class RenderTarget
{
public:
	virtual void GetRenderTargetInternal(void* target) const = 0;
	virtual void Clear(const Colorf& color) = 0;

	RenderTarget(void);
	virtual ~RenderTarget(void);
};

class WindowRenderTarget : public RenderTarget
{
public:
	WindowRenderTarget(void);
	virtual ~WindowRenderTarget(void);

	virtual void Present(void) = 0;
private:

};