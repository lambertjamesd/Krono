
#include "ResourceManager.h"
#include "Mesh/MeshLoader.h"
#include "ObjMeshGroupLoader.h"
#include "Texture2DLoader.h"
#include "ShaderLoader.h"
#include "JSON/JsonDocumentLoader.h"
#include "Compositor/CompositeStageLoader.h"
#include "Compositor/CompositorLoader.h"

namespace krono
{

ResourceManager::ResourceManager(Graphics* graphics) :
	mGraphics(graphics),
	mGeometryCache(*graphics)
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

const Mesh::Ptr& ResourceManager::GetPlane()
{
	return mGeometryCache.GetPlane();
}

void ResourceManager::AddDefaultLoaders()
{
	AddLoader<Mesh>(Auto<ResourceLoader>(new MeshLoader()));
	AddLoader<MeshGroup>(Auto<ResourceLoader>(new ObjMeshGroupLoader()));
	AddLoader<Texture2D>(Auto<ResourceLoader>(new Texture2DLoader()));
	AddLoader<VertexShader>(Auto<ResourceLoader>(new VertexShaderLoader()));
	AddLoader<PixelShader>(Auto<ResourceLoader>(new PixelShaderLoader()));
	AddLoader<JsonDocument>(Auto<ResourceLoader>(new JsonDocumentLoader()));
	AddLoader<CompositeStageLoader>(Auto<ResourceLoader>(new CompositeStageLoader()));
	AddLoader<CompositorLoader>(Auto<ResourceLoader>(new CompositorLoader()));
}

}
