#include "PositionalLight.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

PositionalLight::PositionalLight(GameObject& parentGameObject) :
	Light(parentGameObject),
	Renderable(mGameObject.GetScene().GetRenderManager()),

	mEntity(*mRenderManager.CreateEntity())
{

}

PositionalLight::~PositionalLight(void)
{

}

void PositionalLight::PreRender()
{
	mEntity.SetTransform(mGameObject.GetTransform()->GetWorldTransform());
}
