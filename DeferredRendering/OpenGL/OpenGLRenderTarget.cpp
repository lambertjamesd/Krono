

#include "OpenGLRenderTarget.h"


OpenGLRenderTarget::OpenGLRenderTarget(void) :
	mObjectID(OpenGLObjectIDGenerator::GetNextID())
{
}


OpenGLRenderTarget::~OpenGLRenderTarget(void)
{
}

OpenGLObjectID OpenGLRenderTarget::GetObjectID() const
{
	return mObjectID;
}