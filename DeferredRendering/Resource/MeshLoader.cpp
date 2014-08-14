
#include "MeshLoader.h"
#include "MeshGroup.h"
#include "ResourceManager.h"

MeshLoader::MeshLoader(void)
{
}


MeshLoader::~MeshLoader(void)
{
}

Auto<Resource> MeshLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	return resourceManager.LoadResource<MeshGroup>(inputStream, "")->GetMesh(internalName);
}