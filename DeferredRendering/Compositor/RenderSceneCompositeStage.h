#pragma once

#include "CompositeStage.h"
#include "Interface/ConstantBuffer.h"

namespace krono
{

struct SceneViewData
{
	SceneViewData();

	Matrix4f projectionMatrix;
	Matrix4f viewMatrix;
	Matrix4f projectionViewMatrix;
};

class RenderSceneCompositeStage : public CompositeStage
{
public:
	typedef Auto<RenderSceneCompositeStage> Ptr;

	RenderSceneCompositeStage(size_t technique);
	virtual ~RenderSceneCompositeStage(void);
	
	virtual void Render(RenderState& renderState);
private:
	void RebuildBuffer(Graphics& graphics, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix);
	Auto<ConstantBuffer> mConstantBuffer;

	size_t mTechnique;
};

}
