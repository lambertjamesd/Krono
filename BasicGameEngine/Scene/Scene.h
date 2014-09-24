#pragma once

#include "Render/RenderManager.h"
#include "GameObject/GameObject.h"
#include "UpdateManager.h"
#include <vector>

namespace kge
{

class Game;

class Scene
{
public:
	typedef std::shared_ptr<Scene> Ptr;

	~Scene(void);

	GameObject::Ref CreateGameObject();

	RenderManager& GetRenderManager();
	UpdateManager& GetUpdateManager();

	Game& GetGame();
	krono::ResourceManager& GetResourceManager();
private:
	friend class Game;

	Scene(Game& game);

	Game& mGame;
	RenderManager mRenderManager;
	UpdateManager mUpdateManager;

	std::vector<GameObject::Ptr> mGameObjects;
};

}
