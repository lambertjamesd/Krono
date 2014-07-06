#pragma once
#include "Color.h"
#include "..\Math\Vector2.h"
#include "Texture2D.h"
#include "..\Core\Memory.h"

struct RenderTargetFormat
{
	enum 

	Size size;
};

class RenderTarget
{
public:
	virtual Auto<Texture2D> GetTexture() const = 0;
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