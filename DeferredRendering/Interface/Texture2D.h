#pragma once
#include "Texture.h"
#include "Math/Vector2.h"
#include "DataFormat.h"

namespace krono
{

class Texture2D : public Texture
{
public:
	typedef Auto<Texture2D> Ptr;

	virtual ~Texture2D(void);

	Vector2i GetSize() const;
	DataFormat GetFormat() const;

	static const Ptr Null;
protected:
	Texture2D(const Vector2i& size, DataFormat format);

	Vector2i mSize;
	DataFormat mFormat;
};

}
