#include "BlendState.h"

namespace krono
{

RenderTargetBlend::RenderTargetBlend() :
	blendEnabled(false),
	srcBlend(Blend::SrcAlpha),
	destBlend(Blend::SrcAlphaInv),
	blendOperation(BlendOperator::Add),

	srcBlendAlpha(Blend::SrcAlpha),
	destBlendAlpha(Blend::SrcAlphaInv),
	blendOperationAlpha(BlendOperator::Add),

	blendMask(BlendMask::All)
{

}

BlendStateDescription::BlendStateDescription() :
	alphaToCoverage(false),
	independentBlend(false),
	sampleMask(~0)
{

}

BlendState::BlendState(const BlendStateDescription& description) :
	mDescription(description)
{

}

BlendState::~BlendState(void)
{

}

const BlendStateDescription& BlendState::GetDescription() const
{
	return mDescription;
}

void BlendState::SetBlendFactor(const Colorf& blendFactor)
{
	mDescription.blendFactor = blendFactor;
}

}