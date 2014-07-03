#pragma once
#include "Texture.h"
#include "..\Math\Vector2.h"
#include "DataFormat.h"

class Texture2D : public Texture
{
public:
	virtual ~Texture2D(void);

	Size GetSize() const;
	DataFormat GetFormat() const;

protected:
	Texture2D(const Size& size, DataFormat format);

	Size mSize;
	DataFormat mFormat;
};