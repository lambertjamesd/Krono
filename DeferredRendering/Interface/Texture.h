#pragma once

#include "Resource/Resource.h"

class Texture : public Resource
{
public:
	Texture(void);
	virtual ~Texture(void);

	virtual void LoadMemory(void* source) = 0;
};

