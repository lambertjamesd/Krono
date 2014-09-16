
#include "Renderable.h"
#include "RenderManager.h"


Renderable::Renderable(RenderManager& manager) :
	mRenderManager(manager)
{
	mRenderManager.AddRenderable(this);
}


Renderable::~Renderable(void)
{
	mRenderManager.RemoveRenderable(this);
}