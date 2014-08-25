#pragma once

#include "Render/RenderManager.h"
#include "GameObject/GameObject.h"
#include "UpdateManager.h"
#include <vector>

class Scene
{
public:
	Scene(const krono::Graphics::Ptr& graphics);
	~Scene(void);

	GameObject::Ref CreateGameObject();

	RenderManager& GetRenderManager();
	UpdateManager& GetUpdateManager();
private:
	RenderManager mRenderManager;
	UpdateManager mUpdateManager;

	std::vector<GameObject::Ptr> mGameObjects;
};

