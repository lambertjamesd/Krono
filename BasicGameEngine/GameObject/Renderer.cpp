#include "Renderer.h"

#include "GameObject.h"
#include "Scene/Scene.h"
#include "Serialization/SceneSerializer.h"

namespace kge
{

using namespace krono;

Renderer::Renderer(GameObject& parentGameObject) :
	Component(parentGameObject),
	Renderable(mGameObject.GetScene().GetRenderManager()),

	mEntity(*mRenderManager.CreateEntity())
{

}

Renderer::~Renderer(void)
{
	mRenderManager.RemoveEntity(&mEntity);
}

void Renderer::PreRender()
{
	mEntity.SetTransform(mGameObject.GetTransform()->GetWorldTransform());
}

const Mesh::Ptr& Renderer::GetMesh() const
{
	return mEntity.GetMesh();
}

void Renderer::SetMesh(const Mesh::Ptr& value)
{
	mEntity.SetMesh(value);
}

size_t Renderer::GetMaterialCount() const
{
	return mEntity.GetMaterialCount();
}

const Material::Ptr& Renderer::GetMaterial(size_t index) const
{
	return mEntity.GetMaterial(index);
}

void Renderer::SetMaterial(const Material::Ptr& material, size_t index)
{
	mEntity.SetMaterial(material, index);
}

void Renderer::Serialize(SceneSerializer& serializer)
{
	SerializeEntity(serializer, mEntity);
}

void Renderer::Deserialize(SceneDeserializer& deserializer)
{
	DeserializeEntity(deserializer, mEntity);
}

void Renderer::SerializeEntity(SceneSerializer& serializer, krono::Entity& entity)
{
	serializer.WriteResource("mesh", entity.GetMesh());

	serializer.BeginArray("materials");

	for (size_t i = 0; i < entity.GetMaterialCount(); ++i)
	{
		serializer.WriteResource(entity.GetMaterial(i));
	}

	serializer.EndArray();
}

void Renderer::DeserializeEntity(SceneDeserializer& deserializer, krono::Entity& entity)
{
	entity.SetMesh(deserializer.ReadResource<Mesh>("mesh"));

	size_t materialCount = deserializer.BeginArray("materials");

	for (size_t i = 0; i < materialCount; ++i)
	{
		entity.SetMaterial(deserializer.ReadResource<Material>(), i);
	}

	deserializer.EndArray();
}

}
