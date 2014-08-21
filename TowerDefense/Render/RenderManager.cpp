
#include "RenderManager.h"


RenderManager::RenderManager(void)
{

}

RenderManager::~RenderManager(void)
{

}

krono::Entity* RenderManager::CreateEntity()
{
	return mScene.CreateEntity();
}

void RenderManager::RemoveEntity(krono::Entity* entity)
{
	mScene.RemoveEntity(entity);
}

void RenderManager::AddRenderable(Renderable *renderable)
{
	mRenderables.push_back(renderable);
}

void RenderManager::RemoveRenderable(Renderable *renderable)
{
	mRenderables.erase(std::remove(mRenderables.begin(), mRenderables.end(), renderable), mRenderables.end());
}