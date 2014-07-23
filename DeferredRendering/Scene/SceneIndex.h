#pragma once

#include "../Collide/Frustrum.h"
#include "Entity.h"
#include <functional>

class SceneIndex
{
public:
	SceneIndex(void);
	~SceneIndex(void);

	typedef std::function<void (Entity&)> IndexHitCallback;

	virtual void CollideFrustrum(const Frustrum& frustrum, IndexHitCallback hitCallback) = 0;
	
	virtual void InsertEntity(Auto<Entity>& entity) = 0;
	virtual void UpdateEntity(Entity* entity) = 0;
	virtual void RemoveEntity(Entity* entity) = 0;
private:
};

