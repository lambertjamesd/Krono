
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

PixelCompositeData::PixelCompositeData() :
	projectionMatrix(Matrix4f::Identity()),
	projectionInverseMatrix(Matrix4f::Identity())
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
	RebuildBuffer(renderState);
	renderState.PushConstantBuffer(mVertexContantBuffer, ShaderStage::VertexShader);
	renderState.PushConstantBuffer(mPixelContantBuffer, ShaderStage::PixelShader);

	PushStateParameters(renderState);

	mScreenMesh->GetSubMesh(0)->Render(renderState.GetGraphics());
}

void ScreenQuadCompositeStage::RebuildBuffer(RenderState& renderState)
{
	Matrix4f projectionMatrix = renderState.GetProjectionMatrix();
	Rectf viewport = renderState.GetViewport();
	Vector2f screenSize = renderState.GetRenderTargetSize();
	Matrix4f compositeTransform = Matrix4f::Scale(Vector3f(screenSize / viewport.size, 1.0f)) * Matrix4f::Translation(Vector3f(-viewport.topLeft, 0.0f));

	if (mVertexContantBuffer == NULL)
	{
		ConstantBufferLayout vertexLayout;
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(VertexCompositeData, projectionMatrix));
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeInvProjectionMatrix, offsetof(VertexCompositeData, projectionInverseMatrix));
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(VertexCompositeData, compositeTransform));
		mVertexContantBuffer = renderState.GetGraphics().CreateConstantBuffer(vertexLayout);
		
		ConstantBufferLayout pixelLayout;
		pixelLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(PixelCompositeData, projectionMatrix));
		pixelLayout.MarkSpecialType(ConstantBufferLayout::TypeInvProjectionMatrix, offsetof(PixelCompositeData, projectionInverseMatrix));
		mPixelContantBuffer = renderState.GetGraphics().CreateConstantBuffer(pixelLayout);
	}

	VertexCompositeData vertexData;
	vertexData.projectionMatrix = projectionMatrix;
	vertexData.projectionInverseMatrix = projectionMatrix.Inverse();
	vertexData.compositeTransform = compositeTransform;
	mVertexContantBuffer->Set<VertexCompositeData>(vertexData);
	
	PixelCompositeData pixelData;
	pixelData.projectionMatrix = projectionMatrix;
	pixelData.projectionInverseMatrix = projectionMatrix.Inverse();
	pixelData.screenSize = Vector4f(screenSize, 1.0f / screenSize);
	mPixelContantBuffer->Set<PixelCompositeData>(pixelData);
}

}
