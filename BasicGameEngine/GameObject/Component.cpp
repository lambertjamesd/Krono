
#include "Component.h"

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

}
