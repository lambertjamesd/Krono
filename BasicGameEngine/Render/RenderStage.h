#pragma once

#include <Krono.h>

namespace kge
{

class RenderStage
{
public:
	typedef Auto<RenderStage> Ptr;

	RenderStage(krono::Scene& scene, const std::string& compositor);
	~RenderStage(void);

	void Render(krono::Graphics& graphics, const krono::Compositor::Ptr& compositor);

	void SetViewport(const krono::Rectf& viewport, const krono::Rangef& depthRange);
	const krono::Rectf& GetViewportRect() const;
	const krono::Rangef& GetViewportDepth() const;
	
	void SetViewMatrix(const krono::Matrix4f& value);
	void SetProjectionMatrix(const krono::Matrix4f& value);

	void SetRenderTargets(const krono::RenderTargetConfiguration& renderTarget);
	const krono::RenderTargetConfiguration& GetRenderTargets() const;

	void SetCompositorName(const std::string& name);
	const std::string& GetCompositorName() const;
private:
	krono::SceneView mSceneView;
	std::string mCompositorName;
	krono::RenderTargetConfiguration mRenderTarget;
};

}
