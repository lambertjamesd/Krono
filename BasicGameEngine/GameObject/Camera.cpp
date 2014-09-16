#include "Camera.h"

#include "GameObject.h"
#include "Scene/Scene.h"

Camera::Camera(GameObject& parentGameObject) :
	Component(parentGameObject),
	Renderable(mGameObject.GetScene().GetRenderManager()),

	mRenderStage(mRenderManager.CreateRenderStage()),
	mLens(new OrthographicLens(0.0f, 1.0f, 2.0f))
{

}

Camera::~Camera(void)
{
	mRenderManager.RemoveRenderStage(mRenderStage);
}

void Camera::PreRender()
{
	mRenderStage->SetViewMatrix(mGameObject.GetTransform()->GetInverseWorldTransform());
	mRenderStage->SetProjectionMatrix(mLens->GetProjectionMatrix());
}

void Camera::SetViewport(const krono::Rectf& viewport, const krono::Rangef& depthRange)
{
	mRenderStage->SetViewport(viewport, depthRange);
}

void Camera::SetRenderTargets(const krono::RenderTargetConfiguration& renderTarget)
{
	mRenderStage->SetRenderTargets(renderTarget);
}

void Camera::SetLens(std::unique_ptr<Lens>& lens)
{
	mLens = std::move(lens);
}
