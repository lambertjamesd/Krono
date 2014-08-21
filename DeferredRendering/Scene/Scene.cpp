
#include "Scene.h"
#include "SceneIndexArray.h"

namespace krono
{

Scene::Scene(void) :
	mSceneIndex(new SceneIndexArray())
{

}

Scene::~Scene(void)
{


}

void Scene::CollideFrustrum(const Frustrum& frustrum, SceneIndex::IndexHitCallback hitCallback)
{
	mSceneIndex->CollideFrustrum(frustrum, hitCallback);
}

Entity* Scene::CreateEntity()
{
	Auto<Entity> result(new Entity(mSceneIndex.get()));
	mSceneIndex->InsertEntity(result);
	return result.get();
}

void Scene::RemoveEntity(Entity* entity)
{
	mSceneIndex->RemoveEntity(entity);
}

}