#pragma once

#include "MeshGroup.h"
#include "Core/Memory.h"
#include "Resource/ResourceLoader.h"
#include <iostream>
#include <string>

namespace krono
{

class MeshGroupLoader : public ResourceLoader
{
public:
	MeshGroupLoader(void);
	virtual ~MeshGroupLoader(void);
	
	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName) = 0;
private:
};

}
