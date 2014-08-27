#include "CompositeStage.h"

namespace krono
{

CompositeStage::CompositeStage(void) :
	mExpectedTargetInputCount(0)
{
}


CompositeStage::~CompositeStage(void)
{
}

RenderStateParameters& CompositeStage::GetStateParameters()
{
	return mStateParameters;
}

void CompositeStage::SetExpectedTargetInputCount(size_t value)
{
	mExpectedTargetInputCount = value;
}

size_t CompositeStage::GetExpectedTargetInputCount() const
{
	return mExpectedTargetInputCount;
}

void CompositeStage::PushStateParameters(RenderState& renderState) const
{
	renderState.PushParameters(mStateParameters);
}

}
