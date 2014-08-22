#include "Renderer.h"

#include "GameObject.h"
#include "Scene/Scene.h"

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
	mEntity.SetTransform(mTransform->GetWorldTransform());
}

const Mesh::Ptr& Renderer::GetMesh() const
{
	return mEntity.GetMesh();
}

void Renderer::SetMesh(Mesh::Ptr& value)
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
