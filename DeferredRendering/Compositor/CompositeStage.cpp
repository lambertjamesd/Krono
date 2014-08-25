#include "CompositeStage.h"

namespace krono
{

CompositeStage::CompositeStage(void)
{
}


CompositeStage::~CompositeStage(void)
{
}

const RenderTargetDescription& CompositeStage::GetTargetDescription() const
{
	return mRenderTargetDescription;
}

}