#pragma once

#include "OpenGLFramebuffer.h"
#include <map>
#include "Core/Types.h"

namespace krono
{

class OpenGLFBODatabase
{
public:
	OpenGLFBODatabase(void);
	~OpenGLFBODatabase(void);

	OpenGLFramebuffer& GetFrameBuffer(const std::vector<Auto<OpenGLRenderTarget> > &renderBuffers, const OpenGLDepthBuffer* depthBuffer);
private:
	std::map<UInt32, Auto<OpenGLFramebuffer> > mExistingBuffers;
};

}
