#include "Camera.h"

#include "GameObject.h"
#include "Scene/Scene.h"

Camera::Camera(GameObject& parentGameObject) :
	Component(parentGameObject),
	Renderable(mGameObject.GetScene().GetRenderManager()),

	mRenderStage(mRenderManager.CreateRenderStage())
{

}

Camera::~Camera(void)
{
	mRenderManager.RemoveRenderStage(mRenderStage);
}

void Camera::PreRender()
{
	mRenderStage->SetViewMatrix(mGameObject.GetTransform()->GetInverseWorldTransform());
}

void Camera::SetViewport(const krono::Rectf& viewport, const krono::Rangef& depthRange)
{
	mRenderStage->SetViewport(viewport, depthRange);
}

void Camera::SetRenderTargets(const krono::RenderTargetConfiguration& renderTarget)
{
	mRenderStage->SetRenderTargets(renderTarget);
}