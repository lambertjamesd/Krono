#include "PositionalLight.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "GameObject/Renderer.h"
#include "Serialization/SceneSerializer.h"

using namespace krono;

namespace kge
{

PositionalLight::PositionalLight(GameObject& parentGameObject) :
	Light(parentGameObject),
	Renderable(mGameObject.GetScene().GetRenderManager()),

	mEntity(*mRenderManager.CreateEntity()),
	mRange(1.0f)
{

}

PositionalLight::~PositionalLight(void)
{

}

void PositionalLight::PreRender()
{
	const Matrix4f& worldTransform = mGameObject.GetTransform()->GetWorldTransform().RemoveStretchSkew();
	mEntity.SetTransform(worldTransform * Matrix4f::Scale(Vector3f(mRange, mRange, mRange)));
	SetVariable<Vector3f>("lightPosition", worldTransform.GetTranslation());
	float scaledRadius = mRange * Math<float>::Pow(worldTransform.At(0, 0) * worldTransform.At(1, 1) * worldTransform.At(2, 2), Constant<float>::One / 3);
	SetVariable<float>("lightRange", scaledRadius);
}

void PositionalLight::SetRange(float value)
{
	mRange = value;
}

void PositionalLight::SetColor(const Colorf& value)
{
	SetVariable<Colorf>("lightColor", value);
}

void PositionalLight::Serialize(SceneSerializer& serializer)
{
	Renderer::SerializeEntity(serializer, mEntity);
	serializer.WriteFloat("range", mRange);
}

void PositionalLight::Deserialize(SceneDeserializer& deserializer)
{
	Renderer::DeserializeEntity(deserializer, mEntity);
	mRange = deserializer.ReadFloat("range", mRange);
}


}
