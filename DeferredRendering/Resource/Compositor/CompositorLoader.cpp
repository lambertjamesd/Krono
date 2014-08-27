#include "CompositorLoader.h"

namespace krono
{

CompositorLoader::CompositorLoader(void)
{
}


CompositorLoader::~CompositorLoader(void)
{
}

Auto<Object> CompositorLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	return Auto<Object>(NULL);
}

}