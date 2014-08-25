#include "RenderTargetConfiguration.h"
#include <cassert>

namespace krono
{

RenderTargetConfiguration::RenderTargetConfiguration(void)
{
}


RenderTargetConfiguration::~RenderTargetConfiguration(void)
{
}

void RenderTargetConfiguration::AddRenderTarget(const RenderTarget::Ptr& renderTarget)
{
	if (renderTarget != NULL)
	{
		if (mSize.IsZero())
		{
			mSize = renderTarget->GetSize();
		}
		else
		{
			assert(mSize == renderTarget->GetSize());
		}
	}

	mRenderTargets.push_back(renderTarget);
}

void RenderTargetConfiguration::SetDepthBuffer(const DepthBuffer::Ptr& depthBuffer)
{
	if (depthBuffer != NULL)
	{
		if (mSize.IsZero())
		{
			mSize = depthBuffer->GetSize();
		}
		else
		{
			assert(mSize == depthBuffer->GetSize());
		}
	}

	mDepthBuffer = depthBuffer;
}

const std::vector<RenderTarget::Ptr>& RenderTargetConfiguration::GetRenderTargets() const
{
	return mRenderTargets;
}

const DepthBuffer::Ptr& RenderTargetConfiguration::GetDepthBuffer() const
{
	return mDepthBuffer;
}

const Vector2i& RenderTargetConfiguration::GetSize() const
{
	return mSize;
}

}