
#include "ResourceManager.h"
#include "Mesh/MeshLoader.h"
#include "Mesh/ObjMeshGroupLoader.h"
#include "Texture2DLoader.h"
#include "ShaderLoader.h"
#include "JSON/JsonDocumentLoader.h"
#include "Compositor/CompositeStageLoader.h"
#include "Compositor/CompositorLoader.h"
#include "TextureLoader.h"
#include "MaterialLoader.h"
#include "SamplerLoader.h"

namespace krono
{

ResourceManager::ResourceManager(Graphics* graphics) :
	mGraphics(graphics)
{
	AddDefaultLoaders();
	mPathStack.push_back("");
}


ResourceManager::~ResourceManager(void)
{
}

Graphics* ResourceManager::GetGraphics()
{
	return mGraphics;
}

Mesh::Ptr ResourceManager::GetPlane()
{
	if (!mPlaneCache) {
		mPlaneCache = LoadResource<Mesh>("Engine/Meshes/BuiltInShapes.obj#Plane");
	}

	return mPlaneCache;
}

Mesh::Ptr ResourceManager::GetSphere()
{
	if (!mSphereCache) {
		mSphereCache = LoadResource<Mesh>("Engine/Meshes/BuiltInShapes.obj#Sphere");
	}

	return mSphereCache;
}

void ResourceManager::AddDefaultLoaders()
{
	AddLoader<Mesh>(Auto<ResourceLoader>(new MeshLoader()));
	AddLoader<MeshGroup>(Auto<ResourceLoader>(new ObjMeshGroupLoader()));
	AddLoader<Texture2D>(Auto<ResourceLoader>(new Texture2DLoader()));
	AddLoader<Texture>(Auto<ResourceLoader>(new TextureLoader()));
	AddLoader<VertexShader>(Auto<ResourceLoader>(new VertexShaderLoader()));
	AddLoader<PixelShader>(Auto<ResourceLoader>(new PixelShaderLoader()));
	AddLoader<JsonDocument>(Auto<ResourceLoader>(new JsonDocumentLoader()));
	AddLoader<CompositeStage>(Auto<ResourceLoader>(new CompositeStageLoader()));
	AddLoader<Compositor>(Auto<ResourceLoader>(new CompositorLoader()));
	AddLoader<Material>(Auto<ResourceLoader>(new MaterialLoader()));
	AddLoader<Sampler>(Auto<ResourceLoader>(new SamplerLoader()));
}

}
