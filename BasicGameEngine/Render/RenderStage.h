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
	
	void SetViewMatrix(const krono::Matrix4f& value);
	void SetProjectionMatrix(const krono::Matrix4f& value);

	void SetRenderTargets(const krono::RenderTargetConfiguration& renderTarget);

	const std::string& GetCompositorName() const;
private:
	krono::SceneView mSceneView;
	std::string mCompositorName;
	krono::RenderTargetConfiguration mRenderTarget;
};

}
