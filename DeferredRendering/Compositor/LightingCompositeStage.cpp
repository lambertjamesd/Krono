#include "LightingCompositeStage.h"
#include <cstddef>

namespace krono
{
	
LightVertexCompositeData::LightVertexCompositeData() :
	projectionMatrix(Matrix4f::Identity()),
	projectionViewInverseMatrix(Matrix4f::Identity()),
	compositeTransform(Matrix4f::Identity()),
	projectionMatrixRasterSpace(Matrix4f::Identity())
{

}

LightPixelCompositeData::LightPixelCompositeData() :
	projectionMatrix(Matrix4f::Identity()),
	projectionViewInverseMatrix(Matrix4f::Identity())
{

}

LightingCompositeStage::LightingCompositeStage(size_t technique) :
	mTechnique(technique)
{

}

LightingCompositeStage::~LightingCompositeStage(void)
{

}

void LightingCompositeStage::Render(RenderState& renderState)
{
	RebuildBuffer(renderState);
	
	renderState.PushConstantBuffer(mVertexContantBuffer, ShaderStage::VertexShader);
	renderState.PushConstantBuffer(mPixelContantBuffer, ShaderStage::PixelShader);
	
	PushStateParameters(renderState);
	
	renderState.RenderScene(mTechnique);
}

void LightingCompositeStage::RebuildBuffer(RenderState& renderState)
{
	Matrix4f projectionMatrix = renderState.GetProjectionMatrix();
	Matrix4f viewMatrix = renderState.GetViewMatrix();
	Rectf viewport = renderState.GetViewport();
	Vector2f screenSize = renderState.GetCurrentRenderTargetSize();
	Matrix4f compositeTransform = Matrix4f::Scale(Vector3f(screenSize / viewport.size, 1.0f)) * Matrix4f::Translation(Vector3f(-viewport.topLeft / viewport.size, 0.0f));

	if (mVertexContantBuffer == NULL)
	{
		ConstantBufferLayout vertexLayout;
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(LightVertexCompositeData, projectionMatrix));
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeInvProjectionMatrix, offsetof(LightVertexCompositeData, projectionViewInverseMatrix));
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(LightVertexCompositeData, compositeTransform));
		vertexLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(LightVertexCompositeData, projectionMatrixRasterSpace));
		mVertexContantBuffer = renderState.GetGraphics().CreateConstantBuffer(vertexLayout);
		
		ConstantBufferLayout pixelLayout;
		pixelLayout.MarkSpecialType(ConstantBufferLayout::TypeProjectionMatrix, offsetof(LightPixelCompositeData, projectionMatrix));
		pixelLayout.MarkSpecialType(ConstantBufferLayout::TypeInvProjectionMatrix, offsetof(LightPixelCompositeData, projectionViewInverseMatrix));
		mPixelContantBuffer = renderState.GetGraphics().CreateConstantBuffer(pixelLayout);
	}

	LightVertexCompositeData vertexData;
	vertexData.projectionMatrix = projectionMatrix;
	vertexData.projectionViewInverseMatrix = (projectionMatrix * viewMatrix).Inverse();
	vertexData.compositeTransform = compositeTransform;
	vertexData.projectionMatrixRasterSpace = Matrix4f::Translation(Vector3f(0.5f, 0.5f, 0.0f)) * Matrix4f::Scale(Vector3f(0.5f, -0.5f, 1.0f)) * vertexData.compositeTransform * projectionMatrix;
	mVertexContantBuffer->Set<LightVertexCompositeData>(vertexData);
	
	LightPixelCompositeData pixelData;
	pixelData.projectionMatrix = projectionMatrix;
	pixelData.projectionViewInverseMatrix = vertexData.projectionViewInverseMatrix;
	pixelData.screenSize = Vector4f(screenSize, 1.0f / screenSize);
	pixelData.viewDireciton = Vector4f(0.0f, 0.0f, 1.0f, 0.0f) * viewMatrix;
	pixelData.viewPosition = viewMatrix.Inverse() *  Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	mPixelContantBuffer->Set<LightPixelCompositeData>(pixelData);
}

}
