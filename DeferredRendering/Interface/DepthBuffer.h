#pragma once

#include "Math/Vector2.h"
#include "Texture2D.h"
#include "Core/Memory.h"
#include "DataFormat.h"
#include "Core/Object.h"

namespace krono
{

class DepthBuffer : public Object
{
public:
	typedef Auto<DepthBuffer> Ptr;

	virtual ~DepthBuffer(void);

	DataFormat::Type GetFormat() const;
	Vector2i GetSize() const;
	
	virtual Auto<Texture2D> GetTexture() const = 0;
	virtual void Clear(float value) = 0;

	static Auto<DepthBuffer> Null;
protected:
	DepthBuffer(Vector2i size, DataFormat::Type format);
	
	Vector2i mSize;
	DataFormat::Type mFormat;
};

}
