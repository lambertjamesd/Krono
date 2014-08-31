#pragma once

#include "ResourceLoader.h"

namespace krono
{

class TextureLoader : public ResourceLoader
{
public:
	TextureLoader(void);
	~TextureLoader(void);
	
	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
};

}

