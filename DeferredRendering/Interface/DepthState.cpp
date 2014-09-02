#include "DepthState.h"

namespace krono
{

StencilFunction::StencilFunction() :
		stencilFail(StencilOperation::Keep),
		stencilDepthFail(StencilOperation::Keep),
		stencilPass(StencilOperation::Keep),
		stencilFunction(CompareFunction::Always)
{

}

DepthStateDescription::DepthStateDescription() :
		depthEnable(true),
		depthWriteEnabled(true),
		depthCompare(CompareFunction::Less),

		stencilEnable(false),
		stencilReadMask(~0),
		stencilWriteMask(~0)
{

}

DepthState::DepthState(const DepthStateDescription& description) :
	mDescription(description)
{
}

DepthState::~DepthState(void)
{
}

const DepthStateDescription& DepthState::GetDescription() const
{
	return mDescription;
}

}
