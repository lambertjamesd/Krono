#include "JsonDocumentLoader.h"

#include "JsonDocument.h"
#include "Resource/FormatException.h"

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

json::Value JsonDocumentLoader::LoadJson(std::istream& inputStream)
{
	try
	{
		return json::Deserialize(StringFromIStream(inputStream));
	}
	catch (std::exception&)
	{
		throw FormatException("Invalid Json");
	}
}

bool JsonDocumentLoader::IsJsonDocument(std::istream& inputStream)
{
	int characterRead;

	auto lastPosition = inputStream.tellg();

	do
	{
		characterRead = inputStream.get();

		if (characterRead == '{')
		{
			inputStream.seekg(lastPosition, std::ios_base::beg);
			return true;
		}

	} while (isspace(characterRead));

	inputStream.seekg(lastPosition, std::ios_base::beg);

	return false;
}

}
