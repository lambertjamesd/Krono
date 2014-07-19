#include "stdafx.h"
#include "OpenGLFramebuffer.h"
#include <iostream>

OpenGLFramebuffer::OpenGLFramebuffer(void) :
	mFrameBuffer(0)
{
	mBuffers.push_back(GL_BACK_LEFT);
}

OpenGLFramebuffer::OpenGLFramebuffer(const std::vector<Auto<OpenGLRenderTarget> > &renderBuffers, const OpenGLDepthBuffer* depthBuffer)
{
	if (renderBuffers.size() == 1 && renderBuffers[0]->GetType() == OpenGLRenderTarget::TypeWindow && depthBuffer == NULL)
	{
		mFrameBuffer = 0;
		mBuffers.push_back(GL_BACK_LEFT);
	}
	else
	{
		GLuint existingBuffer;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&existingBuffer);

		glGenFramebuffers(1, &mFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

		size_t currentAttachment = GL_COLOR_ATTACHMENT0;

		for (auto it = renderBuffers.cbegin(); it != renderBuffers.cend(); ++it)
		{
			const OpenGLRenderTarget* currentBuffer = it->get();

			if (currentBuffer == NULL)
			{
				mBuffers.push_back(GL_NONE);
			}
			else if (currentBuffer->GetType() == OpenGLRenderTarget::TypeWindow)
			{
				mBuffers.push_back(GL_BACK_LEFT);
			}
			else
			{
				BindBuffer(currentBuffer, currentAttachment);
				mBuffers.push_back(currentAttachment);
				++currentAttachment;
			}
		}

		if (depthBuffer != NULL)
		{
			BindBuffer(depthBuffer, GL_DEPTH_ATTACHMENT);

			if (renderBuffers.size() ==  0)
			{
				mBuffers.push_back(GL_NONE);
			}
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Error creating frame buffer";
		}

		glBindFramebuffer(GL_FRAMEBUFFER, existingBuffer);
	}
}

OpenGLFramebuffer::~OpenGLFramebuffer(void)
{

}

void OpenGLFramebuffer::Use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	glDrawBuffers(mBuffers.size(), &mBuffers.front());
}

void OpenGLFramebuffer::BindBuffer(const OpenGLRenderTarget* buffer, GLenum attachment)
{
	if (buffer->GetType() == OpenGLRenderTarget::TypeTexture)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, buffer->GetGLObject(), 0);
	}
	else if (buffer->GetType() == OpenGLRenderTarget::TypeRenderBuffer)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffer->GetGLObject());
	}
}