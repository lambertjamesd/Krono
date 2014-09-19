#pragma once

namespace kge
{

class RenderManager;

class Renderable
{
public:
	virtual ~Renderable(void);

	virtual void PreRender() = 0;
protected:
	Renderable(RenderManager& manager);
	RenderManager &mRenderManager;
};

}
