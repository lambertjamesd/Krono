#include "TextureLoader.h"
#include "Interface/Texture2D.h"
#include "FormatException.h"
#include "ResourceManager.h"

namespace krono
{

TextureLoader::TextureLoader(void)
{
}


TextureLoader::~TextureLoader(void)
{
}


Auto<Object> TextureLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	if (internalName == "2D" || internalName == "")
	{
		return resourceManager.LoadResource<Texture2D>(inputStream, "");
	}
	else
	{
		throw FormatException("unrecognized texture dimension");
	}
}

}