#pragma once
#include "Color.h"
#include "Math/Vector2.h"
#include "Texture2D.h"
#include "Core/Memory.h"
#include "Core/Object.h"
#include "DataFormat.h"

namespace krono
{

class RenderTarget : public Object
{
public:
	typedef Auto<RenderTarget> Ptr;

	virtual Auto<Texture2D> GetTexture() const = 0;
	virtual void Clear(const Colorf& color) = 0;

	virtual ~RenderTarget(void);

	Vector2i GetSize() const;
	const DataFormat& GetDataFormat() const;

	static const Ptr Null;
protected:
	RenderTarget(const Vector2i& size, const DataFormat& dataFormat);
private:
	Vector2i mSize;
	DataFormat mDataFormat;
};

class WindowRenderTarget : public RenderTarget
{
public:
	typedef Auto<WindowRenderTarget> Ptr;

	virtual ~WindowRenderTarget(void);

	virtual void Present(void) = 0;
protected:
	WindowRenderTarget(const Vector2i& size, const DataFormat& dataFormat);
private:

};

}
