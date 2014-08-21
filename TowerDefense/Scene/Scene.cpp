
#include "Scene.h"


Scene::Scene(void)
{
}


Scene::~Scene(void)
{
}

GameObject::Ref Scene::CreateGameObject()
{
	GameObject::Ptr result(new GameObject(this));
	result->mSelfReference = result;
	mGameObjects.push_back(result);
	return result;
}