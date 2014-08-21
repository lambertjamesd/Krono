#pragma once

#include "GameObject/GameObject.h"
#include <vector>

class Scene
{
public:
	Scene(void);
	~Scene(void);

	GameObject::Ref CreateGameObject();
private:
	std::vector<GameObject::Ptr> mGameObjects;
};

