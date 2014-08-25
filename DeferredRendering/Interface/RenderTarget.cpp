
#include "RenderTarget.h"

namespace krono
{

RenderTarget::RenderTarget(const Vector2i& size) :
	mSize(size)
{
}


RenderTarget::~RenderTarget(void)
{
}

Vector2i RenderTarget::GetSize() const
{
	return mSize;
}

const RenderTarget::Ptr RenderTarget::Null = NULL;

WindowRenderTarget::WindowRenderTarget(const Vector2i& size) :
	RenderTarget(size)
{

}

WindowRenderTarget::~WindowRenderTarget(void)
{

}

}
