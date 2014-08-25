#include "RenderSceneCompositeStage.h"

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

	renderState.PushConstantBuffer(mEntityBuffer, ShaderStage::VertexShader);
	
	renderState.RenderScene(mTechnique);

	renderState.PopState();
}


void RenderSceneCompositeStage::RebuildBuffer(Graphics& graphics, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix)
{
	if (mEntityBuffer == NULL)
	{
		ConstantBufferLayout layout;
		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(SceneViewData, projectionMatrix));
		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(SceneViewData, projectionViewMatrix));
		mEntityBuffer = graphics.CreateConstantBuffer(layout);
	}

	SceneViewData data;
	data.projectionMatrix = projectionMatrix;
	data.viewMatrix = viewMatrix;
	data.projectionViewMatrix = projectionMatrix * viewMatrix;

	mEntityBuffer->Set<SceneViewData>(data);
}

}