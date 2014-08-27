#pragma once

#include "CompositeStage.h"
#include "Interface/ConstantBuffer.h"
#include "Resource/Mesh/Mesh.h"
#include "RenderStateParameters.h"

namespace krono
{

struct VertexCompositeData
{
	VertexCompositeData();

	Matrix4f projectionMatrix;
	Matrix4f projectionInverseMatrix;
	Matrix4f compositeTransform;
};

struct PixelCompositeData
{
	PixelCompositeData();

	Matrix4f projectionMatrix;
	Matrix4f projectionInverseMatrix;
};

class ScreenQuadCompositeStage : public CompositeStage
{
public:
	ScreenQuadCompositeStage(const Mesh::Ptr& screenMesh);
	~ScreenQuadCompositeStage(void);
	
	virtual void Render(RenderState& renderState);
private:
	void RebuildBuffer(Graphics& graphics, const Matrix4f& compositeTransform, const Matrix4f& projectionMatrix);
	Auto<ConstantBuffer> mVertexContantBuffer;
	Auto<ConstantBuffer> mPixelContantBuffer;

	Mesh::Ptr mScreenMesh;
};

}
