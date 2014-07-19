#pragma once

#include "ResourceLoader.h"

class MeshLoader : public ResourceLoader
{
public:
	MeshLoader(void);
	~MeshLoader(void);
	
	virtual Auto<Resource> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
private:
};

