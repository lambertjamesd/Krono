
#include "Scene.h"
#include "Game.h"

namespace kge
{

Scene::Scene(Game& game) : 
	mGame(game),
	mRenderManager(game.GetGraphics())
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

Game& Scene::GetGame()
{
	return mGame;
}

krono::ResourceManager& Scene::GetResourceManager()
{
	return *mGame.GetResourceManager();
}

}
