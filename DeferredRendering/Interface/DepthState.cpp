#include "DepthState.h"

namespace krono
{

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