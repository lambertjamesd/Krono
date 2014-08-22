
#include "SceneView.h"
#include <stddef.h>

namespace krono
{

SceneViewData::SceneViewData() :
	projectionMatrix(Matrix4f::Identity()),
	viewMatrix(Matrix4f::Identity()),
	projectionViewMatrix(Matrix4f::Identity())
{

}

SceneView::SceneView(Scene& scene) :
	mScene(scene),
	mBufferIsDirty(true),
	mProjectionViewIsDirty(false),
	mTechniqueType(0)
{

}

SceneView::~SceneView(void)
{

}

void SceneView::SetViewMatrix(const Matrix4f& viewMatrix)
{
	mSceneData.viewMatrix = viewMatrix;
	mProjectionViewIsDirty = true;
}

void SceneView::SetProjectionMatrix(const Matrix4f& projectionMatrix)
{
	mSceneData.projectionMatrix = projectionMatrix;
	mProjectionViewIsDirty = true;
}

void SceneView::Render(Graphics& graphics)
{
	RebuildBuffer(graphics);

	Frustrum viewFrustrum(mSceneData.projectionViewMatrix);

	graphics.SetConstantBuffer(mSceneViewBuffer, SCENEVIEW_DATA_INDEX, ShaderStage::PixelShader);
	graphics.SetConstantBuffer(mSceneViewBuffer, SCENEVIEW_DATA_INDEX, ShaderStage::VertexShader);

	mScene.CollideFrustrum(viewFrustrum, [&](Entity& entity) {
		entity.Render(graphics, mTechniqueType);
	});
}

void SceneView::RebuildBuffer(Graphics& graphics)
{
	if (mSceneViewBuffer == NULL)
	{
		ConstantBufferLayout layout;

		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(SceneViewData, projectionMatrix));
		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(SceneViewData, projectionViewMatrix));

		mSceneViewBuffer = Auto<ConstantBuffer>(graphics.CreateConstantBuffer(layout));
	}

	if (mBufferIsDirty)
	{
		if (mProjectionViewIsDirty)
		{
			mSceneData.projectionViewMatrix = mSceneData.projectionMatrix * mSceneData.viewMatrix;
		}

		mSceneViewBuffer->Set<SceneViewData>(mSceneData);
		mBufferIsDirty = false;
	}
}

}
