
#include "RenderManager.h"
#include <algorithm>

using namespace std;

RenderManager::RenderManager(const krono::Graphics::Ptr& graphics) :
	mGraphics(graphics),
	mGeometryCache(*graphics)
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

RenderStage::Ptr RenderManager::CreateRenderStage()
{
	RenderStage::Ptr result(new RenderStage(mScene, mDefaultCompositor));
	mRenderStages.push_back(result);
	return result;
}

void RenderManager::RemoveRenderStage(const RenderStage::Ptr& stage)
{
	mRenderStages.erase(remove(mRenderStages.begin(), mRenderStages.end(), stage), mRenderStages.end());
}

void RenderManager::SetDefaultCompositor(const krono::Compositor::Ptr& compositor)
{
	mDefaultCompositor = compositor;
}

GeometryCache& RenderManager::GetGeometryCache()
{
	return mGeometryCache;
}

void RenderManager::Render()
{
	for (auto it = mRenderables.begin(); it != mRenderables.end(); ++it)
	{
		(*it)->PreRender();
	}

	for (auto it = mRenderStages.begin(); it != mRenderStages.end(); ++it)
	{
		(*it)->Render(*mGraphics);
	}
}

void RenderManager::AddRenderable(Renderable *renderable)
{
	mRenderables.push_back(renderable);
}

void RenderManager::RemoveRenderable(Renderable *renderable)
{
	mRenderables.erase(std::remove(mRenderables.begin(), mRenderables.end(), renderable), mRenderables.end());
}
