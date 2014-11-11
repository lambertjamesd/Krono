#pragma once

#include "Component.h"
#include "Render/Renderable.h"
#include "Transform.h"
#include <Krono.h>
namespace kge
{

class Renderer : public Component, public Renderable
{
public:
	typedef std::shared_ptr<Renderer> Ptr;
	typedef std::weak_ptr<Renderer> Ref;

	Renderer(GameObject& parentGameObject);
	~Renderer(void);
	
	virtual void PreRender();

	const krono::Mesh::Ptr& GetMesh() const;
	void SetMesh(const krono::Mesh::Ptr& value);

	size_t GetMaterialCount() const;

	const krono::Material::Ptr& GetMaterial(size_t index) const;
	void SetMaterial(const krono::Material::Ptr& material, size_t index);
	
	virtual void Serialize(SceneSerializer& serializer);
	virtual void Deserialize(SceneDeserializer& deserializer);

	static void SerializeEntity(SceneSerializer& serializer, krono::Entity& entity);
	static void DeserializeEntity(SceneDeserializer& deserializer, krono::Entity& entity);
private:
	krono::Entity& mEntity;
};

}
