
#include "SceneView.h"
#include <stddef.h>

namespace krono
{

SceneView::SceneView(Scene& scene) :
	mScene(scene),
	mProjectionMatrix(Matrix4f::Identity()),
	mViewMatrix(Matrix4f::Identity()),

	mNormalizedViewport(Vector2f(), Vector2f(1.0f, 1.0f)),
	mDepthRange(0.0f, 1.0f)
{

}

SceneView::~SceneView(void)
{

}

Scene& SceneView::GetScene()
{
	return mScene;
}

void SceneView::SetViewport(const krono::Rectf& viewport, const krono::Rangef& depthRange)
{
	mNormalizedViewport = viewport;
	mDepthRange = depthRange;
}

const krono::Rectf& SceneView::GetViewport() const
{
	return mNormalizedViewport;
}

const krono::Rangef& SceneView::GetDepthRange() const
{
	return mDepthRange;
}

void SceneView::SetViewMatrix(const Matrix4f& viewMatrix)
{
	mViewMatrix = viewMatrix;
}

void SceneView::SetProjectionMatrix(const Matrix4f& projectionMatrix)
{
	mProjectionMatrix = projectionMatrix;
}

const Matrix4f& SceneView::GetViewMatrix() const
{
	return mViewMatrix;
}

const Matrix4f& SceneView::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

Matrix4f SceneView::CalculateProjectionMatrix(const Vector2f& viewportSize) const
{
	float aspectRatio = viewportSize.y / viewportSize.x;
	return Matrix4f::Scale(Vector3f(aspectRatio, 1.0f, 1.0f)) * mProjectionMatrix;
}

Rectf SceneView::CalculateViewport(const Vector2i& renderTargetSize) const
{
	return Rectf(mNormalizedViewport.topLeft * (Vector2f)renderTargetSize, mNormalizedViewport.size * (Vector2f)renderTargetSize);
}

}
