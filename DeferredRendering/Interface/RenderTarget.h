#pragma once
#include "Color.h"
#include "Math/Vector2.h"
#include "Texture2D.h"
#include "Core/Memory.h"

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

	virtual ~RenderTarget(void);

	Vector2i GetSize() const;
protected:
	RenderTarget(const Vector2i& size);
private:
	Vector2i mSize;
};

class WindowRenderTarget : public RenderTarget
{
public:
	virtual ~WindowRenderTarget(void);

	virtual void Present(void) = 0;
protected:
	WindowRenderTarget(const Vector2i& size);
private:

};