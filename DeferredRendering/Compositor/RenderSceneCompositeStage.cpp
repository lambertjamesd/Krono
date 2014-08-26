#include "RenderSceneCompositeStage.h"
#include <cstddef>

namespace krono
{

SceneViewData::SceneViewData() :
	projectionMatrix(Matrix4f::Identity()),
	viewMatrix(Matrix4f::Identity()),
	projectionViewMatrix(Matrix4f::Identity())
{

}

RenderSceneCompositeStage::RenderSceneCompositeStage(size_t technique) :
	mTechnique(technique)
{
}


RenderSceneCompositeStage::~RenderSceneCompositeStage(void)
{
}

void RenderSceneCompositeStage::Render(RenderState& renderState)
{
	renderState.PushState();

	Matrix4f viewMatrix = renderState.GetViewMatrix();
	Matrix4f projectionMatrix = renderState.GetProjectionMatrix();

	RebuildBuffer(renderState.GetGraphics(), viewMatrix, projectionMatrix);

	renderState.PushConstantBuffer(mConstantBuffer, ShaderStage::VertexShader);
	
	renderState.RenderScene(mTechnique);

	renderState.PopState();
}


void RenderSceneCompositeStage::RebuildBuffer(Graphics& graphics, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix)
{
	if (mConstantBuffer == NULL)
	{
		ConstantBufferLayout layout;
		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(SceneViewData, projectionMatrix));
		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(SceneViewData, projectionViewMatrix));
		mConstantBuffer = graphics.CreateConstantBuffer(layout);
	}

	SceneViewData data;
	data.projectionMatrix = projectionMatrix;
	data.viewMatrix = viewMatrix;
	data.projectionViewMatrix = projectionMatrix * viewMatrix;

	mConstantBuffer->Set<SceneViewData>(data);
}

}
