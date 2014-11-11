#pragma once

#include "Core/Object.h"
#include "Core/Memory.h"

#include "Resource/Resource.h"

namespace krono
{

class Texture : public Resource
{
public:
	typedef Auto<Texture> Ptr;

	Texture(void);
	virtual ~Texture(void);

	virtual void LoadMemory(void* source) = 0;

	virtual void GenerateMipmaps() = 0;
	
	static const Ptr Null;
};

}
