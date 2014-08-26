#pragma once

#include "CompositeStage.h"
#include "Interface/ConstantBuffer.h"
#include "Resource/Mesh/Mesh.h"
#include "RenderStateParameters.h"

namespace krono
{

struct CompositeData
{
	CompositeData();

	Matrix4f projectionMatrix;
	Matrix4f projectionInverseMatrix;
	Matrix4f compositeTransform;
};

class ScreenQuadCompositeStage : public CompositeStage
{
public:
	ScreenQuadCompositeStage(const Mesh::Ptr& screenMesh, const RenderStateParameters& renderParameters);
	~ScreenQuadCompositeStage(void);
	
	virtual void Render(RenderState& renderState);
private:
	void RebuildBuffer(Graphics& graphics, const Matrix4f& compositeTransform, const Matrix4f& projectionMatrix);
	Auto<ConstantBuffer> mConstantBuffer;

	Mesh::Ptr mScreenMesh;
	RenderStateParameters mRenderParameters;
};

}
