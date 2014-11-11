
#include "RenderManager.h"
#include <algorithm>

#include "Serialization/SceneSerializer.h"

using namespace krono;

namespace kge
{

using namespace std;

RenderManager::RenderManager(const krono::Graphics::Ptr& graphics, Scene& gameObjectScene) :
	mGraphics(graphics),
	mGameObjectScene(gameObjectScene)
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
	RenderStage::Ptr result(new RenderStage(mScene, DefaultCompositor));
	mRenderStages.push_back(result);
	return result;
}

void RenderManager::RemoveRenderStage(const RenderStage::Ptr& stage)
{
	mRenderStages.erase(remove(mRenderStages.begin(), mRenderStages.end(), stage), mRenderStages.end());
}

void RenderManager::SetCompositor(const std::string& name, const krono::Compositor::Ptr& compositor)
{
	mCompositors[name] = compositor;
}

const krono::Compositor::Ptr& RenderManager::GetCompositor(const std::string& name) const
{
	auto result = mCompositors.find(name);

	if (result == mCompositors.end())
	{
		return Compositor::Null;
	}
	else
	{
		return result->second;
	}
}

const krono::Graphics::Ptr& RenderManager::GetGraphics() const
{
	return mGraphics;
}

Scene& RenderManager::GetGameObjectScene()
{
	return mGameObjectScene;
}

const std::string RenderManager::DefaultCompositor = "default";

void RenderManager::Render()
{
	for (auto it = mRenderables.begin(); it != mRenderables.end(); ++it)
	{
		(*it)->PreRender();
	}

	for (auto it = mRenderStages.begin(); it != mRenderStages.end(); ++it)
	{
		Compositor::Ptr compositor = mCompositors[(*it)->GetCompositorName()];

		if (compositor == NULL)
		{
			throw Exception(std::string("Could not find compositor named " + (*it)->GetCompositorName()));
		}
		else
		{
			(*it)->Render(*mGraphics, compositor);
		}
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

}
