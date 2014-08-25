#pragma once

#include <vector>
#include "Interface/RenderTarget.h"
#include "Interface/DepthBuffer.h"

namespace krono
{

class RenderTargetConfiguration
{
public:
	RenderTargetConfiguration(void);
	~RenderTargetConfiguration(void);

	void AddRenderTarget(const RenderTarget::Ptr& renderTarget);
	void SetDepthBuffer(const DepthBuffer::Ptr& depthBuffer);

	const std::vector<RenderTarget::Ptr>& GetRenderTargets() const;
	const DepthBuffer::Ptr& GetDepthBuffer() const;
	const Vector2i& GetSize() const;
private:
	std::vector<RenderTarget::Ptr> mRenderTargets;
	DepthBuffer::Ptr mDepthBuffer;

	Vector2i mSize;
};

}