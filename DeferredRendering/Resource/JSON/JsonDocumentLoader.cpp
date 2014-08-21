#include "JsonDocumentLoader.h"

#include "JsonDocument.h"

namespace krono
{

JsonDocumentLoader::JsonDocumentLoader(void)
{
}


JsonDocumentLoader::~JsonDocumentLoader(void)
{
}

Auto<Object> JsonDocumentLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	return Auto<Object>(new JsonDocument(StringFromIStream(inputStream)));
}

}
