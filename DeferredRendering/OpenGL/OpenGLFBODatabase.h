#pragma once

#include "OpenGLFramebuffer.h"
#include <map>

class OpenGLFBODatabase
{
public:
	OpenGLFBODatabase(void);
	~OpenGLFBODatabase(void);

	OpenGLFramebuffer& GetFrameBuffer(const std::vector<Auto<OpenGLRenderTarget> > &renderBuffers, const OpenGLDepthBuffer* depthBuffer);
private:
	std::map<UINT32, Auto<OpenGLFramebuffer> > mExistingBuffers;
};

