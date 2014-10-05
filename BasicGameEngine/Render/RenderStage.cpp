#include "RenderStage.h"

namespace kge
{

using namespace krono;

RenderStage::RenderStage(Scene& scene, const std::string& compistor) :
	mSceneView(scene),
	mCompositorName(compistor)
{

}

RenderStage::~RenderStage(void)
{

}

void RenderStage::Render(Graphics& graphics, const krono::Compositor::Ptr& compositor)
{
	compositor->Render(graphics, mRenderTarget, mSceneView);
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

const std::string& RenderStage::GetCompositorName() const
{
	return mCompositorName;
}

}
