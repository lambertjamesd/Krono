
#include "ScreenQuadCompositeStage.h"
#include <cstddef>

namespace krono
{

CompositeData::CompositeData() :
	projectionMatrix(Matrix4f::Identity()),
	projectionInverseMatrix(Matrix4f::Identity()),
	compositeTransform(Matrix4f::Identity())
{

}

ScreenQuadCompositeStage::ScreenQuadCompositeStage(const Mesh::Ptr& screenMesh) :
	mScreenMesh(screenMesh)
{

}

ScreenQuadCompositeStage::~ScreenQuadCompositeStage(void)
{
	
}

void ScreenQuadCompositeStage::Render(RenderState& renderState)
{
	renderState.PushState();

	RebuildBuffer(renderState.GetGraphics(), Matrix4f::Identity(), renderState.GetProjectionMatrix());
	renderState.PushConstantBuffer(mConstantBuffer, ShaderStage::VertexShader);
	renderState.PushConstantBuffer(mConstantBuffer, ShaderStage::PixelShader);

	PushTargetInput(renderState);

	mScreenMesh->GetSubMesh(0)->Render(renderState.GetGraphics());

	renderState.PopState();
}

void ScreenQuadCompositeStage::RebuildBuffer(Graphics& graphics, const Matrix4f& compositeTransform, const Matrix4f& projectionMatrix)
{
	if (mConstantBuffer == NULL)
	{
		ConstantBufferLayout layout;
		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(CompositeData, projectionMatrix));
		layout.MarkSpecialType(ConstantBufferLayout::TypeInvProjectionMatrix, offsetof(CompositeData, projectionInverseMatrix));
		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(CompositeData, compositeTransform));
		mConstantBuffer = graphics.CreateConstantBuffer(layout);
	}

	CompositeData data;
	data.projectionMatrix = projectionMatrix;
	data.projectionInverseMatrix = projectionMatrix.Inverse();
	data.compositeTransform = compositeTransform;

	mConstantBuffer->Set<CompositeData>(data);
}

}
