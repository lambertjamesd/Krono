#pragma once

#include "Component.h"
#include "Render/Renderable.h"
#include "Transform.h"
#include <Krono.h>

class Renderer : public Component, public Renderable
{
public:
	typedef std::shared_ptr<Renderer> Ptr;
	typedef std::weak_ptr<Renderer> Ref;

	Renderer(GameObject& parentGameObject);
	~Renderer(void);
	
	virtual void PreRender();

	const Mesh::Ptr& GetMesh() const;
	void SetMesh(Mesh::Ptr& value);

	size_t GetMaterialCount() const;

	const Material::Ptr& GetMaterial(size_t index) const;
	void SetMaterial(const Material::Ptr& material, size_t index);
private:
	krono::Entity& mEntity;
	Transform::Ptr mTransform;
};

