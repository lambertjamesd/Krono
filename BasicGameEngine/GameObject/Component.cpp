
#include "Component.h"
#include "GameObject.h"
#include "Scene/Scene.h"
#include "Scene/Game.h"

namespace kge
{

Component::Component(GameObject& parentGameObject) :
	mGameObject(parentGameObject)
{

}

Component::~Component(void)
{
}

GameObject& Component::GetGameObject()
{
	return mGameObject;
}

krono::ResourceManager& Component::GetResourceManager()
{
	return *mGameObject.GetScene().GetGame().GetResourceManager();
}

const krono::InputState& Component::GetInputState() const
{
	return mGameObject.GetScene().GetGame().GetInputState();
}

}
