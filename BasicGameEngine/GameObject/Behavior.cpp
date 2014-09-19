#include "Behavior.h"
#include "GameObject.h"
#include "Scene/Scene.h"

namespace kge
{

Behavior::Behavior(GameObject& parentGameObject) :
	Component(parentGameObject),
	Updatable(mGameObject.GetScene().GetUpdateManager())
{
}


Behavior::~Behavior(void)
{
}

void Behavior::Update(float deltaTime)
{

}

}
