#include "stdafx.h"
#include "Component.h"


Component::Component(GameObject* parentGameObject) :
	mGameObject(parentGameObject)
{

}


Component::~Component(void)
{
}
