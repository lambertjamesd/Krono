#include "Camera.h"

#include "GameObject.h"
#include "Scene/Scene.h"

#include "Serialization/SceneSerializer.h"
#include "Serialization/RenderSerialization.h"

using namespace krono;

namespace kge
{

Camera::Camera(GameObject& parentGameObject) :
	Component(parentGameObject),
	Renderable(mGameObject.GetScene().GetRenderManager()),

	mRenderStage(mRenderManager.CreateRenderStage()),
	mIsPerspective(false),
	mNearPlane(0.0f),
	mFarPlane(1.0f),
	mHeight(2.0f)
{

}

Camera::~Camera(void)
{
	mRenderManager.RemoveRenderStage(mRenderStage);
}

void Camera::PreRender()
{
	mRenderStage->SetViewMatrix(mGameObject.GetTransform()->GetInverseWorldTransform().RemoveScale());

	if (mIsPerspective)
	{
		mRenderStage->SetProjectionMatrix(mPostProjectionMatrix * Matrix4f::PerspectiveProjection(mNearPlane, mFarPlane, Degreesf(mHeight), 1.0f));
	}
	else
	{
		mRenderStage->SetProjectionMatrix(mPostProjectionMatrix * Matrix4f::OrthoProjection(mNearPlane, mFarPlane, mHeight, 1.0f));
	}
}

void Camera::SetViewport(const krono::Rectf& viewport, const krono::Rangef& depthRange)
{
	mRenderStage->SetViewport(viewport, depthRange);
}

void Camera::SetRenderTargets(const krono::RenderTargetConfiguration& renderTarget)
{
	mRenderStage->SetRenderTargets(renderTarget);
}

void Camera::SetOrthoLens(float nearPlane, float farPlane, float height)
{
	mIsPerspective = false;
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
	mHeight = height;
}

void Camera::SetPerspectiveLens(float nearPlane, float farPlane, const krono::Degreesf& vFov)
{
	mIsPerspective = true;
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
	mHeight = vFov;
}

void Camera::SetPostProjectionMatrix(const krono::Matrix4f& value)
{
	mPostProjectionMatrix = value;
}

const krono::Matrix4f& Camera::GetPostProjectionMatrix() const
{
	return mPostProjectionMatrix;
}

void Camera::Serialize(SceneSerializer& serializer)
{
	serializer.WriteKey("renderStage");
	RenderSerialization(mRenderManager).SerializeRenderStage(serializer, mRenderStage);

	serializer.WriteBool("perspective", mIsPerspective);
	serializer.WriteFloat("near", mNearPlane);
	serializer.WriteFloat("far", mFarPlane);
	serializer.WriteFloat("height", mHeight);
}

void Camera::Deserialize(SceneDeserializer& deserializer)
{
	deserializer.ReadKey("renderStage");
	RenderSerialization(mRenderManager).DeserializeRenderStage(deserializer, mRenderStage);

	mIsPerspective = deserializer.ReadBool("perspective", mIsPerspective);
	mNearPlane = deserializer.ReadFloat("near", mNearPlane);
	mFarPlane = deserializer.ReadFloat("far", mFarPlane);
	mHeight = deserializer.ReadFloat("height", mHeight);
}

}
