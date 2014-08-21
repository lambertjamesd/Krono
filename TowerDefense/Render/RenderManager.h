#pragma once

#include <Krono.h>

#include "Renderable.h"
#include <vector>

class RenderManager
{
public:
	RenderManager(void);
	~RenderManager(void);

	krono::Entity* CreateEntity();
	void RemoveEntity(krono::Entity* entity);

private:
	friend class Renderable;

	void AddRenderable(Renderable *renderable);
	void RemoveRenderable(Renderable *renderable);

	krono::Scene mScene;

	std::vector<Renderable*> mRenderables;
};

