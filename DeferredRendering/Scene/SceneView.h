#pragma once

#include "Scene.h"
#include "Math/Matrix.h"
#include "Core/Memory.h"
#include "Interface/Graphics.h"
#include "Interface/ConstantBuffer.h"

namespace krono
{

class SceneView
{
public:
	SceneView(Scene& scene);
	~SceneView(void);

	Scene& GetScene();
	
	void SetViewport(const krono::Rectf& viewport, const krono::Rangef& depthRange);
	const krono::Rectf& GetViewport() const;
	const krono::Rangef& GetDepthRange() const;

	void SetViewMatrix(const Matrix4f& viewMatrix);
	void SetProjectionMatrix(const Matrix4f& projectionMatrix);

	const Matrix4f& GetViewMatrix() const;
	const Matrix4f& GetProjectionMatrix() const;

	Matrix4f CalculateProjectionMatrix(const Vector2f& renderTargetSize) const;
	Rectf CalculateViewport(const Vector2i& renderTargetSize) const;
private:
	Scene& mScene;
	Matrix4f mProjectionMatrix;
	Matrix4f mViewMatrix;
	
	Rectf mNormalizedViewport;
	Rangef mDepthRange;
};

}