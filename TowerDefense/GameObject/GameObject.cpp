#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject(void)
{
	AddComponent<Transform>();
}


GameObject::~GameObject(void)
{
}
