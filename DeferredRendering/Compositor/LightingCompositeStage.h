#pragma once

#include "CompositeStage.h"

namespace krono
{

struct LightVertexCompositeData
{
	LightVertexCompositeData();

	Matrix4f projectionMatrix;
	Matrix4f projectionInverseMatrix;
	Matrix4f compositeTransform;
	Matrix4f projectionMatrixRasterSpace;
};

struct LightPixelCompositeData
{
	LightPixelCompositeData();

	Matrix4f projectionMatrix;
	Matrix4f projectionInverseMatrix;
	Vector4f screenSize;
};

class LightingCompositeStage : public CompositeStage
{
public:
	typedef Auto<LightingCompositeStage> Ptr;

	LightingCompositeStage(size_t technique);
	~LightingCompositeStage(void);
	
	virtual void Render(RenderState& renderState);
private:
	void RebuildBuffer(RenderState& renderState);
	Auto<ConstantBuffer> mVertexContantBuffer;
	Auto<ConstantBuffer> mPixelContantBuffer;

	size_t mTechnique;
};

}
