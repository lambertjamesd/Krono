
#include "RenderTarget.h"

namespace krono
{

RenderTarget::RenderTarget(const Vector2i& size, const DataFormat& dataFormat) :
	mSize(size),
	mDataFormat(dataFormat)
{
}


RenderTarget::~RenderTarget(void)
{
}

Vector2i RenderTarget::GetSize() const
{
	return mSize;
}

const DataFormat& RenderTarget::GetDataFormat() const
{
	return mDataFormat;
}

const RenderTarget::Ptr RenderTarget::Null = NULL;

WindowRenderTarget::WindowRenderTarget(const Vector2i& size, const DataFormat& dataFormat) :
	RenderTarget(size, dataFormat)
{

}

WindowRenderTarget::~WindowRenderTarget(void)
{

}

}
