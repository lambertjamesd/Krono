#include "RenderState.h"

namespace krono
{

RenderState::RenderState(Graphics& graphics) :
	mGraphics(graphics)
{

}


RenderState::~RenderState(void)
{

}

void RenderState::PushParameters(RenderStateParameters& parameters)
{
	const RenderStateParameters::ParameterCount& parameterCount = parameters.GetParameterCount();
	mParameterCount.push_back(parameterCount);

	for (size_t stage = 0; stage < ShaderStage::TypeCount; ++stage)
	{
		for (size_t texture = 0; texture < parameterCount.textureCount[stage]; ++texture)
		{
			mGraphics.SetTexture(parameters.GetTexture(static_cast<ShaderStage::Type>(stage), texture), mCurrentTextureSlot[stage], static_cast<ShaderStage::Type>(stage));
			++mCurrentTextureSlot[stage];
		}
	}
}

void RenderState::PopParameters()
{
	const RenderStateParameters::ParameterCount& parameterCount = mParameterCount.back();
	
	for (size_t stage = 0; stage < ShaderStage::TypeCount; ++stage)
	{
		mCurrentTextureSlot[stage] -= parameterCount.textureCount[stage];
		mCurrentUniformBufferSlot[stage] -= parameterCount.uniformCount[stage];
	}

	mParameterCount.pop_back();
}

}