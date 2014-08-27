#pragma once

#include "Resource/ResourceLoader.h"

namespace krono
{

class CompositorLoader : public ResourceLoader
{
public:
	CompositorLoader(void);
	~CompositorLoader(void);
	
	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
private:
};

}

