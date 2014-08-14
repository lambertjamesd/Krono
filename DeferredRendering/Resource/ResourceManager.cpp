
#include "ResourceManager.h"
#include "MeshLoader.h"
#include "ObjMeshGroupLoader.h"
#include "Texture2DLoader.h"

ResourceManager::ResourceManager(Graphics* graphics) :
	mGraphics(graphics)
{
	AddDefaultLoaders();
}


ResourceManager::~ResourceManager(void)
{
}

Graphics* ResourceManager::GetGraphics()
{
	return mGraphics;
}

void ResourceManager::AddDefaultLoaders()
{
	AddLoader<Mesh>(Auto<ResourceLoader>(new MeshLoader()));
	AddLoader<MeshGroup>(Auto<ResourceLoader>(new ObjMeshGroupLoader()));
	AddLoader<Texture2D>(Auto<ResourceLoader>(new Texture2DLoader()));
}