#pragma once

#include "OpenGLFramebuffer.h"
#include <map>

class OpenGLFBODatabase
{
public:
	OpenGLFBODatabase(void);
	~OpenGLFBODatabase(void);

	OpenGLFramebuffer& GetFrameBuffer(const std::vector<Auto<OpenGLRenderTarget> > &renderBuffers);
private:
	std::map<UINT32, Auto<OpenGLFramebuffer> > mExistingBuffers;
};

