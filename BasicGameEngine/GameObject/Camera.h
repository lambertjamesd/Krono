#pragma once
#include "Component.h"
#include "Render/Renderable.h"
#include "Render/RenderStage.h"
#include "Transform.h"
#include <Krono.h>
#include <memory>
namespace kge
{

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

	void SetOrthoLens(float near, float far, float height);
	void SetPerspectiveLens(float near, float far, const krono::Degreesf& vFov);
	
	void SetPostProjectionMatrix(const krono::Matrix4f& value);
	const krono::Matrix4f& GetPostProjectionMatrix() const;
	
	virtual void Serialize(SceneSerializer& serializer);
	virtual void Deserialize(SceneDeserializer& deserializer);
private:
	RenderStage::Ptr mRenderStage;
	krono::Matrix4f mPostProjectionMatrix;

	bool mIsPerspective;
	float mNearPlane;
	float mFarPlane;
	// this is the height for orthographic
	// and the vertical fov in degrees for perspective
	float mHeight;
};

}
