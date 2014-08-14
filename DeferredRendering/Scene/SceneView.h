#pragma once

#include "Scene.h"
#include "Math/Matrix.h"
#include "Core/Memory.h"
#include "Interface/Graphics.h"
#include "Interface/ConstantBuffer.h"

struct SceneViewData
{
	SceneViewData();

	Matrix4f projectionMatrix;
	Matrix4f viewMatrix;
	Matrix4f projectionViewMatrix;
};

class SceneView
{
public:
	SceneView(Scene& scene);
	~SceneView(void);

	void SetViewMatrix(const Matrix4f& viewMatrix);
	void SetProjectionMatrix(const Matrix4f& projectionMatrix);

	void Render(Graphics& graphics);
private:
	void RebuildBuffer(Graphics& graphics);

	static const size_t SCENEVIEW_DATA_INDEX = 0;

	Auto<ConstantBuffer> mSceneViewBuffer;

	Scene& mScene;
	SceneViewData mSceneData;
	bool mBufferIsDirty;
	bool mProjectionViewIsDirty;
	size_t mTechniqueType;
};

