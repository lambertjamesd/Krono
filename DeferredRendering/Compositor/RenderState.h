#pragma once

#include "Interface/Shader.h"
#include "Interface/Graphics.h"
#include "RenderStateParameters.h"
#include <vector>

namespace krono
{

class RenderState
{
public:
	RenderState(Graphics& graphics);
	~RenderState(void);

	void PushParameters(RenderStateParameters& parameters);
	void PopParameters();
private:
	Graphics& mGraphics;

	std::vector<RenderStateParameters::ParameterCount> mParameterCount;

	size_t mCurrentTextureSlot[ShaderStage::TypeCount];
	size_t mCurrentUniformBufferSlot[ShaderStage::TypeCount];
};

}