#include "stdafx.h"
#include "RenderTarget.h"


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

WindowRenderTarget::WindowRenderTarget(const Vector2i& size) :
	RenderTarget(size)
{

}

WindowRenderTarget::~WindowRenderTarget(void)
{

}