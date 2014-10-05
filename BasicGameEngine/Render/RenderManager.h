#pragma once

#include <Krono.h>

#include "Renderable.h"
#include "RenderStage.h"
#include <vector>
#include <string>
#include <map>

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

	void SetCompositor(const std::string& name, const krono::Compositor::Ptr& compositor);
	const krono::Compositor::Ptr& GetCompositor(const std::string& name) const;

	static const std::string DefaultCompositor;

	void Render();
private:
	friend class Renderable;

	void AddRenderable(Renderable *renderable);
	void RemoveRenderable(Renderable *renderable);

	krono::Graphics::Ptr mGraphics;
	std::map<std::string, krono::Compositor::Ptr> mCompositors;

	krono::Scene mScene;

	std::vector<Renderable*> mRenderables;
	std::vector<RenderStage::Ptr> mRenderStages;
};

}
