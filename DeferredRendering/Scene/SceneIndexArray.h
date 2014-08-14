#pragma once

#include "Entity.h"
#include <vector>
#include "Core/Memory.h"
#include "SceneIndex.h"

class SceneIndexArray : public SceneIndex
{
public:
	SceneIndexArray(void);
	~SceneIndexArray(void);
	
	virtual void CollideFrustrum(const Frustrum& frustrum, IndexHitCallback hitCallback);
	
	virtual void InsertEntity(Auto<Entity>& entity);
	virtual void UpdateEntity(Entity* entity);
	virtual void RemoveEntity(Entity* entity);
private:
	std::vector<Auto<Entity> > mEntityList;
};
