#pragma once

#include <Krono.h>

class RenderStage
{
public:
	typedef Auto<RenderStage> Ptr;

	RenderStage(krono::Scene& scene, const krono::Compositor::Ptr& compistor);
	~RenderStage(void);

	void Render(krono::Graphics& graphics);

	void SetViewport(const krono::Rectf& viewport, const krono::Rangef& depthRange);
	
	void SetViewMatrix(const krono::Matrix4f& value);
	void SetProjectionMatrix(const krono::Matrix4f& value);

	void SetRenderTargets(const krono::RenderTargetConfiguration& renderTarget);
private:
	krono::SceneView mSceneView;
	krono::Compositor::Ptr mCompositor;
	krono::RenderTargetConfiguration mRenderTarget;
};

