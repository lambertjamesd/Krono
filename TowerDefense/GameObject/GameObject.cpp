
#include "GameObject.h"
#include "Scene/Scene.h"

GameObject::GameObject(Scene* parentScene) :
	mScene(parentScene)
{
	AddComponent<Transform>();
}


GameObject::~GameObject(void)
{

}

Scene& GameObject::GetScene()
{
	return *mScene;
}