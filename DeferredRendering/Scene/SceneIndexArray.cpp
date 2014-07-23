#include "stdafx.h"
#include "SceneIndexArray.h"


SceneIndexArray::SceneIndexArray(void)
{
}


SceneIndexArray::~SceneIndexArray(void)
{
}


void SceneIndexArray::CollideFrustrum(const Frustrum& frustrum, IndexHitCallback hitCallback)
{
	for (auto it = mEntityList.begin(); it != mEntityList.end(); ++it)
	{
		// TODO bounding box check
		hitCallback(**it);
	}
}

void SceneIndexArray::InsertEntity(Auto<Entity>& entity)
{
	mEntityList.push_back(entity);
}

void SceneIndexArray::UpdateEntity(Entity* entity)
{
	// Array index does nothing
}

void SceneIndexArray::RemoveEntity(Entity* entity)
{
	for (auto it = mEntityList.begin(); it != mEntityList.end(); ++it)
	{
		if (it->get() == entity)
		{
			mEntityList.erase(it);
			break;
		}
	}
}