
#include "MeshLoader.h"
#include "MeshGroup.h"
#include "Resource/ResourceManager.h"

namespace krono
{

MeshLoader::MeshLoader(void)
{
}


MeshLoader::~MeshLoader(void)
{
}

Auto<Object> MeshLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	return resourceManager.LoadResource<MeshGroup>(inputStream, "")->GetMesh(internalName);
}

}
