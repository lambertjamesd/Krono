#pragma once
#include "Component.h"
#include "Render/Renderable.h"
#include "Render/RenderStage.h"
#include "Transform.h"
#include <Krono.h>

class Camera : public Component, public Renderable
{
public:
	typedef std::shared_ptr<Camera> Ptr;
	typedef std::weak_ptr<Camera> Ref;

	Camera(GameObject& parentGameObject);
	~Camera(void);

	virtual void PreRender();
	
	void SetViewport(const krono::Rectf& viewport, const krono::Rangef& depthRange);

	void SetRenderTargets(const krono::RenderTargetConfiguration& renderTarget);
private:
	RenderStage::Ptr mRenderStage;
};