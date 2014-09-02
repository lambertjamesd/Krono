
#include "RasterizerState.h"

namespace krono
{

RasterizerStateDescription::RasterizerStateDescription() :
		fillMode(FillMode::Solid),

		cullMode(CullMode::Back),
		frontCCW(false),

		depthBias(0),
		depthBiasClamp(0.0f),
		slopeScaledDepthBias(0.0f),

		depthClipEnable(true),
		scissorEnable(false),
		multisampleEnable(false),
		antialiasedLineEnable(false)
{

}

RasterizerState::RasterizerState(const RasterizerStateDescription& description) :
		mDescription(description)
{

}

RasterizerState::~RasterizerState()
{

}

const RasterizerStateDescription& RasterizerState::GetDescription() const
{
	return mDescription;
}

}
