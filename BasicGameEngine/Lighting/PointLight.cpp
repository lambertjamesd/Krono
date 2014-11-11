#include "PointLight.h"

#include "GameObject/GameObject.h"

using namespace krono;

namespace kge
{

PointLight::PointLight(GameObject& parentGameObject) :
	PositionalLight(parentGameObject)
{
	mEntity.SetMesh(GetResourceManager().GetSphere());
	mEntity.SetMaterial(GetResourceManager().LoadResource<Material>("Engine/Lights/PointLightMaterial.json"), 0);
	SetVariable("lightColor", Colorf(1.0f, 1.0f, 1.0f, 1.0f));
}


PointLight::~PointLight(void)
{
}

}