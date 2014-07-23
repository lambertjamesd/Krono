#pragma once
#include "SceneIndex.h"
#include <memory>

class Scene
{
public:
	Scene(void);
	~Scene(void);
	
	void CollideFrustrum(const Frustrum& frustrum, SceneIndex::IndexHitCallback hitCallback);

	Entity* CreateEntity();
	void RemoveEntity(Entity* entity);
private:
	Scene(const Scene& other);
	Scene& operator=(const Scene& other);

	std::unique_ptr<SceneIndex> mSceneIndex;
};

