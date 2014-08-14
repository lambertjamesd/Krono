#pragma once
#include "Texture.h"
#include "Math\Vector2.h"
#include "DataFormat.h"

class Texture2D : public Texture
{
public:
	virtual ~Texture2D(void);

	Vector2i GetSize() const;
	DataFormat GetFormat() const;

protected:
	Texture2D(const Vector2i& size, DataFormat format);

	Vector2i mSize;
	DataFormat mFormat;
};