#pragma once

#include <Krono.h>

#include "Renderable.h"
#include "RenderStage.h"
#include <vector>

namespace kge
{

class RenderManager
{
public:
	RenderManager(const krono::Graphics::Ptr& graphics);
	~RenderManager(void);

	krono::Entity* CreateEntity();
	void RemoveEntity(krono::Entity* entity);

	RenderStage::Ptr CreateRenderStage();
	void RemoveRenderStage(const RenderStage::Ptr& stage);

	void SetDefaultCompositor(const krono::Compositor::Ptr& compositor);

	void Render();
private:
	friend class Renderable;

	void AddRenderable(Renderable *renderable);
	void RemoveRenderable(Renderable *renderable);

	krono::Graphics::Ptr mGraphics;
	krono::Compositor::Ptr mDefaultCompositor;

	krono::Scene mScene;

	std::vector<Renderable*> mRenderables;
	std::vector<RenderStage::Ptr> mRenderStages;
};

}
