#include "CompositeStageLoader.h"

namespace krono
{

CompositeStageLoader::CompositeStageLoader(void)
{
}


CompositeStageLoader::~CompositeStageLoader(void)
{
}

Auto<Object> CompositeStageLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	return Auto<Object>(NULL);
}

}