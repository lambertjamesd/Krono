#pragma once

#include "Core/Object.h"

class Texture : public Object
{
public:
	Texture(void);
	virtual ~Texture(void);

	virtual void LoadMemory(void* source) = 0;

	virtual void GenerateMipmaps() = 0;
};

