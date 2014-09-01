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

RenderTargetBlend::RenderTargetBlend(
	Blend::Type srcBlend, Blend::Type destBlend, BlendOperator::Type blendOperation,
	Blend::Type srcBlendAlpha, Blend::Type destBlendAlpha, BlendOperator::Type blendOperationAlpha,
	BlendMask::Type blendMask) :
	blendEnabled(true),
	srcBlend(srcBlend), destBlend(destBlend), blendOperation(blendOperation),
	srcBlendAlpha(srcBlendAlpha), destBlendAlpha(destBlendAlpha), blendOperationAlpha(blendOperationAlpha),
	blendMask(blendMask)
{

}
	
const RenderTargetBlend RenderTargetBlend::AlphaBlend(
	Blend::SrcAlpha, Blend::SrcAlphaInv, BlendOperator::Add,
	Blend::SrcAlpha, Blend::SrcAlphaInv, BlendOperator::Add,
	BlendMask::All);

const RenderTargetBlend RenderTargetBlend::Add(
	Blend::One, Blend::One, BlendOperator::Add,
	Blend::One, Blend::One, BlendOperator::Add,
	BlendMask::All);

const RenderTargetBlend RenderTargetBlend::Multiply(
	Blend::Zero, Blend::SrcColor, BlendOperator::Add,
	Blend::Zero, Blend::SrcAlpha, BlendOperator::Add,
	BlendMask::All);

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