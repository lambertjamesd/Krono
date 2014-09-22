#include "PointLight.h"

#include "GameObject/GameObject.h"

using namespace krono;

namespace kge
{

PointLight::PointLight(GameObject& parentGameObject) :
	PositionalLight(parentGameObject),
	mRadius(1.0f)
{
	mEntity.SetMesh(GetResourceManager().GetSphere());
	mEntity.SetMaterial(GetResourceManager().LoadResource<Material>("Media/Engine/Lights/PointLightMaterial.json"), 0);
	SetVariable("lightColor", Colorf(1.0f, 1.0f, 1.0f, 1.0f));
}


PointLight::~PointLight(void)
{
}


void PointLight::PreRender()
{
	PositionalLight::PreRender();
	Vector3f scale = mGameObject.GetTransform()->GetWorldTransform().TransformDirection(Vector3f::UnitScale());
	float scaledRadius = mRadius * Math<float>::Pow(scale.x * scale.y * scale.z, Constant<float>::One / 3);
	SetVariable<float>("lightRadius", scaledRadius);
}

}