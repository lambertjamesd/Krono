
#include "Scene.h"
#include "Game.h"

namespace kge
{

Scene::Scene(Game& game) : 
	mGame(game),
	mRenderManager(game.GetGraphics(), *this)
{

}


Scene::~Scene(void)
{
}

GameObject::Ref Scene::CreateGameObject()
{
	GameObject::Ptr result(new GameObject(*this));
	result->mSelfReference = result;
	mGameObjects.push_back(result);
	return result;
}

void Scene::RemoveGameObject(GameObject& gameObject)
{
	for (size_t i = 0; i < mGameObjects.size(); ++i)
	{
		if (mGameObjects[i].get() == &gameObject)
		{
			mGameObjects.erase(mGameObjects.begin() + i);
			break;
		}
	}
}

size_t Scene::GetGameObjectCount() const
{
	return mGameObjects.size();
}

GameObject::Ref Scene::GetGameObject(size_t index) const
{
	return mGameObjects[index];
}

RenderManager& Scene::GetRenderManager()
{
	return mRenderManager;
}

UpdateManager& Scene::GetUpdateManager()
{
	return mUpdateManager;
}

Game& Scene::GetGame()
{
	return mGame;
}

krono::ResourceManager& Scene::GetResourceManager()
{
	return *mGame.GetResourceManager();
}

}
