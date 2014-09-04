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

class LightingCompositeState : public CompositeStage
{
public:
	typedef Auto<LightingCompositeState> Ptr;

	LightingCompositeState(size_t technique);
	~LightingCompositeState(void);
	
	virtual void Render(RenderState& renderState);
private:
	void RebuildBuffer(RenderState& renderState);
	Auto<ConstantBuffer> mVertexContantBuffer;
	Auto<ConstantBuffer> mPixelContantBuffer;

	size_t mTechnique;
};

}