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
	
	virtual Auto<Resource> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);

	static json::Value LoadJson(std::istream& inputStream);
	static bool IsJsonDocument(std::istream& inputStream);
private:
};

}
