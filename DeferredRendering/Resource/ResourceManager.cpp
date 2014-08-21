
#include "ResourceManager.h"
#include "Mesh/MeshLoader.h"
#include "ObjMeshGroupLoader.h"
#include "Texture2DLoader.h"
#include "ShaderLoader.h"
#include "JSON/JsonDocumentLoader.h"

namespace krono
{

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
	AddLoader<VertexShader>(Auto<ResourceLoader>(new VertexShaderLoader()));
	AddLoader<PixelShader>(Auto<ResourceLoader>(new PixelShaderLoader()));
	AddLoader<JsonDocument>(Auto<ResourceLoader>(new JsonDocumentLoader()));
}

}
