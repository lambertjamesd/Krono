#pragma once
#include "Interface/Graphics.h"
#include "Core/Object.h"
#include "Resource.h"
#include <iostream>
#include <string>

namespace krono
{

class ResourceManager;

class ResourceLoader
{
public:
	ResourceLoader(void);
	virtual ~ResourceLoader(void);

	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName) = 0;
protected:
	static std::string StringFromIStream(std::istream& inputStream);
private:
};

}
