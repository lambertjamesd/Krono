#pragma once

#include "Resource/ResourceLoader.h"

namespace krono
{

class MeshLoader : public ResourceLoader
{
public:
	MeshLoader(void);
	~MeshLoader(void);
	
	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
private:
};

}
