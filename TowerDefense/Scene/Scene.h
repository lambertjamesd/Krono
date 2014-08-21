#pragma once

#include "Render/RenderManager.h"
#include "GameObject/GameObject.h"
#include <vector>

class Scene
{
public:
	Scene(void);
	~Scene(void);

	GameObject::Ref CreateGameObject();

	RenderManager& GetRenderManager();
private:
	RenderManager mRenderManager;

	std::vector<GameObject::Ptr> mGameObjects;
};

