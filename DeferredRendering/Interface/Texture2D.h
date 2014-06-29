#pragma once
#include "Texture.h"
#include "..\Math\Vector2.h"

class Texture2D : public Texture
{
public:

	enum Format
	{
		RGB8UI,
		RGB16F,
		RGB32F,

		RGBA8UI,
		RGBA16F,
		RGBA32F,
	};

	virtual ~Texture2D(void);

	virtual void Resize(const Size& size) = 0;

	Size GetSize() const;
	Format GetFormat() const;

protected:
	Texture2D(const Size& size, Format format);

	Size mSize;
	Format mFormat;
};