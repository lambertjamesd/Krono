#pragma once

#include "Resource/ResourceLoader.h"
#include "JsonDocument.h"

namespace krono
{

class JsonDocumentLoader : public ResourceLoader
{
public:
	JsonDocumentLoader(void);
	~JsonDocumentLoader(void);
	
	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
private:
};

}
