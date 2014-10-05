
#include "ScreenQuadCompositeStage.h"
#include <cstddef>

namespace krono
{

VertexCompositeData::VertexCompositeData() :
	projectionViewMatrix(Matrix4f::Identity()),
	projectionViewInverseMatrix(Matrix4f::Identity()),
	compositeTransform(Matrix4f::Identity()),
	projectionMatrixRasterSpace(Matrix4f::Identity())
{

}

PixelCompositeData::PixelCompositeData() :
	projectionViewMatrix(Matrix4f::Identity()),
	projectionViewInverseMatrix(Matrix4f::Identity())
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
	Matrix4f viewMatrix = renderState.GetViewMatrix();
	Rectf viewport = renderState.GetViewport();
	Vector2f screenSize = renderState.GetCurrentRenderTargetSize();
	Matrix4f compositeTransform = Matrix4f::Scale(Vector3f(screenSize / viewport.size, 1.0f)) * Matrix4f::Translation(Vector3f(-viewport.topLeft / screenSize, 0.0f));

	if (mVertexContantBuffer == NULL)
	{
		ConstantBufferLayout vertexLayout;
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(VertexCompositeData, projectionViewMatrix));
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeInvProjectionMatrix, offsetof(VertexCompositeData, projectionViewInverseMatrix));
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(VertexCompositeData, compositeTransform));
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(VertexCompositeData, projectionMatrixRasterSpace));
		mVertexContantBuffer = renderState.GetGraphics().CreateConstantBuffer(vertexLayout);
		
		ConstantBufferLayout pixelLayout;
		pixelLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(PixelCompositeData, projectionViewMatrix));
		pixelLayout.MarkSpecialType(ConstantBufferLayout::TypeInvProjectionMatrix, offsetof(PixelCompositeData, projectionViewInverseMatrix));
		mPixelContantBuffer = renderState.GetGraphics().CreateConstantBuffer(pixelLayout);
	}

	VertexCompositeData vertexData;
	vertexData.projectionViewMatrix = projectionMatrix * viewMatrix;
	vertexData.projectionViewInverseMatrix = vertexData.projectionViewMatrix.Inverse();
	vertexData.compositeTransform = compositeTransform;
	vertexData.projectionMatrixRasterSpace = Matrix4f::Translation(Vector3f(0.5f, 0.5f, 0.0f)) * Matrix4f::Scale(Vector3f(0.5f, -0.5f, 1.0f)) * vertexData.compositeTransform * projectionMatrix;
	mVertexContantBuffer->Set<VertexCompositeData>(vertexData);
	
	PixelCompositeData pixelData;
	pixelData.projectionViewMatrix = vertexData.projectionViewMatrix;
	pixelData.projectionViewInverseMatrix = vertexData.projectionViewInverseMatrix;
	pixelData.screenSize = Vector4f(screenSize, 1.0f / screenSize);
	mPixelContantBuffer->Set<PixelCompositeData>(pixelData);
}

}
