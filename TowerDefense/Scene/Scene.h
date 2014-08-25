#pragma once

#include "Render/RenderManager.h"
#include "GameObject/GameObject.h"
#include <vector>

class Scene
{
public:
	Scene(const krono::Graphics::Ptr& graphics);
	~Scene(void);

	GameObject::Ref CreateGameObject();

	RenderManager& GetRenderManager();
private:
	RenderManager mRenderManager;

	std::vector<GameObject::Ptr> mGameObjects;
};

