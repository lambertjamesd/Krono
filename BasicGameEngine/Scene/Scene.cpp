
#include "Scene.h"


Scene::Scene(const krono::Graphics::Ptr& graphics) : 
	mRenderManager(graphics)
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

RenderManager& Scene::GetRenderManager()
{
	return mRenderManager;
}

UpdateManager& Scene::GetUpdateManager()
{
	return mUpdateManager;
}