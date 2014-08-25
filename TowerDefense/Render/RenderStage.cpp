#include "RenderStage.h"

using namespace krono;

RenderStage::RenderStage(Scene& scene, const Compositor::Ptr& compistor) :
	mSceneView(scene),
	mCompositor(compistor)
{

}

RenderStage::~RenderStage(void)
{

}

void RenderStage::Render(Graphics& graphics)
{
	mCompositor->Render(graphics, mRenderTarget, mSceneView);
}

void RenderStage::SetViewport(const krono::Rectf& viewport, const krono::Rangef& depthRange)
{
	mSceneView.SetViewport(viewport, depthRange);
}

void RenderStage::SetViewMatrix(const krono::Matrix4f& value)
{
	mSceneView.SetViewMatrix(value);
}

void RenderStage::SetProjectionMatrix(const krono::Matrix4f& value)
{
	mSceneView.SetProjectionMatrix(value);
}

void RenderStage::SetRenderTargets(const krono::RenderTargetConfiguration& renderTarget)
{
	mRenderTarget = renderTarget;
}