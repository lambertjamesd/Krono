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
	Matrix4f projectionMatrixRasterSpace;
};

struct PixelCompositeData
{
	PixelCompositeData();

	Matrix4f projectionMatrix;
	Matrix4f projectionInverseMatrix;
	Vector4f screenSize;
};

class ScreenQuadCompositeStage : public CompositeStage
{
public:
	typedef Auto<ScreenQuadCompositeStage> Ptr;

	ScreenQuadCompositeStage(const Mesh::Ptr& screenMesh);
	~ScreenQuadCompositeStage(void);
	
	virtual void Render(RenderState& renderState);
private:
	void RebuildBuffer(RenderState& renderState);
	Auto<ConstantBuffer> mVertexContantBuffer;
	Auto<ConstantBuffer> mPixelContantBuffer;

	Mesh::Ptr mScreenMesh;
};

}
