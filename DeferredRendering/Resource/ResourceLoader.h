#pragma once
#include "Interface/Graphics.h"
#include "Resource.h"
#include <iostream>
#include <string>

class ResourceManager;

class ResourceLoader
{
public:
	ResourceLoader(void);
	virtual ~ResourceLoader(void);

	virtual Auto<Resource> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName) = 0;
private:
};

