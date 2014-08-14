#pragma once

#include "MeshGroup.h"
#include "Core/Memory.h"
#include "ResourceLoader.h"
#include <iostream>
#include <string>

class MeshGroupLoader : public ResourceLoader
{
public:
	MeshGroupLoader(void);
	virtual ~MeshGroupLoader(void);
	
	virtual Auto<Resource> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName) = 0;
private:
};

