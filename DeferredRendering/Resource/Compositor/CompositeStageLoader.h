#pragma once

#include "Resource/ResourceLoader.h"

namespace krono
{

class CompositeStageLoader : public ResourceLoader
{
public:
	CompositeStageLoader(void);
	~CompositeStageLoader(void);
	
	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
private:
};

}