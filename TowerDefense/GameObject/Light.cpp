#include "Light.h"

#include "GameObject.h"
#include "Scene/Scene.h"

Light::Light(GameObject& parentGameObject) :
	Component(parentGameObject),
	Renderable(mGameObject.GetScene().GetRenderManager()),

	mEntity(*mRenderManager.CreateEntity())
{

}


Light::~Light(void)
{

}

void Light::PreRender()
{
	mEntity.SetTransform(mGameObject.GetTransform()->GetWorldTransform());
}