#include "PositionalLight.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

using namespace krono;

namespace kge
{

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
	const Matrix4f& worldTransform = mGameObject.GetTransform()->GetWorldTransform();
	mEntity.SetTransform(worldTransform.RemoveStretchSkew());
	SetVariable<Vector3f>("lightPosition", worldTransform.GetTranslation());
}

}
