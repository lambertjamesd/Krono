
#include "ScreenQuadCompositeStage.h"
#include <cstddef>

namespace krono
{

VertexCompositeData::VertexCompositeData() :
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
	RebuildBuffer(renderState.GetGraphics(), Matrix4f::Identity(), renderState.GetProjectionMatrix());
	renderState.PushConstantBuffer(mVertexContantBuffer, ShaderStage::VertexShader);
	renderState.PushConstantBuffer(mPixelContantBuffer, ShaderStage::PixelShader);

	PushStateParameters(renderState);

	mScreenMesh->GetSubMesh(0)->Render(renderState.GetGraphics());
}

void ScreenQuadCompositeStage::RebuildBuffer(Graphics& graphics, const Matrix4f& compositeTransform, const Matrix4f& projectionMatrix)
{
	if (mVertexContantBuffer == NULL)
	{
		ConstantBufferLayout layout;
		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(VertexCompositeData, projectionMatrix));
		layout.MarkSpecialType(ConstantBufferLayout::TypeInvProjectionMatrix, offsetof(VertexCompositeData, projectionInverseMatrix));
		layout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(VertexCompositeData, compositeTransform));
		mVertexContantBuffer = graphics.CreateConstantBuffer(layout);
	}

	VertexCompositeData data;
	data.projectionMatrix = projectionMatrix;
	data.projectionInverseMatrix = projectionMatrix.Inverse();
	data.compositeTransform = compositeTransform;

	mVertexContantBuffer->Set<VertexCompositeData>(data);
}

}
