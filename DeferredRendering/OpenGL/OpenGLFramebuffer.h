#pragma once

#include "..\Core\Memory.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLDepthBuffer.h"
#include <vector>

class OpenGLFramebuffer
{
public:
	OpenGLFramebuffer(void);
	OpenGLFramebuffer(const std::vector<Auto<OpenGLRenderTarget> > &renderBuffers, const OpenGLDepthBuffer* depthBuffer);
	~OpenGLFramebuffer(void);

	void Use();
private:
	void BindBuffer(const OpenGLRenderTarget* buffer, GLenum attachment);

	GLuint mFrameBuffer;
	std::vector<GLenum> mBuffers;
};

