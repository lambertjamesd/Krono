
#include "DirectionalLight.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

using namespace krono;

namespace kge
{


DirectionalLight::DirectionalLight(GameObject& parentGameObject) :
	Light(parentGameObject),
	Renderable(mGameObject.GetScene().GetRenderManager()),
	mEntity(*mRenderManager.CreateEntity())
{
	mEntity.SetMesh(GetResourceManager().GetPlane());
	mEntity.SetMaterial(GetResourceManager().LoadResource<Material>("Media/Engine/Lights/DirectionalLightMaterial.json"), 0);
	SetDirection(Vector3f(0.0f, -1.0f, 0.0f));
	SetColor(Colorf(1.0f, 1.0f, 1.0f, 1.0f));
}

DirectionalLight::~DirectionalLight(void)
{

}

void DirectionalLight::PreRender()
{

}

void DirectionalLight::SetDirection(const krono::Vector3f& direction)
{
	SetVariable<Vector3f>("lightDirection", direction);
}

void DirectionalLight::SetColor(const krono::Colorf& value)
{
	SetVariable<Colorf>("lightColor", value);
}

}
