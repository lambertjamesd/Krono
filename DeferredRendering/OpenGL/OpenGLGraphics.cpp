#include "stdafx.h"
#include "OpenGLGraphics.h"
#include "OpenGLShader.h"
#include "OpenGLShaderProgram.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLWindowRenderTarget.h"
#include <iostream>

OpenGLGraphics::OpenGLGraphics(void) :
		mHasGlewInitialized(false)
{

}


OpenGLGraphics::~OpenGLGraphics(void)
{
}

Auto<VertexBuffer> OpenGLGraphics::CreateVertexBuffer(const InputLayout& inputLayout)
{
	return Auto<VertexBuffer>(new OpenGLVertexBuffer(inputLayout));
}

Auto<VertexShader> OpenGLGraphics::CreateVertexShader(const std::string& source)
{
	return Auto<VertexShader>(new OpenGLVertexShader(source));
}

Auto<FragmentShader> OpenGLGraphics::CreateFragmentShader(const std::string& source)
{
	return Auto<FragmentShader>(new OpenGLFragmentShader(source));
}

Auto<ShaderProgram> OpenGLGraphics::CreateShaderProgram(const Auto<VertexShader>& vertexShader, const Auto<FragmentShader>& fragmentShader)
{
	return Auto<ShaderProgram>(new OpenGLShaderProgram(
		dynamic_cast<OpenGLVertexShader&>(*vertexShader), 
		dynamic_cast<OpenGLFragmentShader&>(*fragmentShader)));
}

Auto<WindowRenderTarget> OpenGLGraphics::CreateWindowRenderTarget(Window& targetWindow)
{
	Auto<WindowRenderTarget> result(new OpenGLWindowRenderTarget(targetWindow));

	if (!mHasGlewInitialized)
	{
		GLenum err = glewInit();

		if (GLEW_OK != err)
		{
			std::cerr << "Could not initialize glew Error: " << glewGetErrorString(err) << std::endl;
		}

		mHasGlewInitialized = true;
	}

	return result;
}

void OpenGLGraphics::Draw(size_t count, size_t offset)
{
	glDrawArrays(GL_TRIANGLES, offset, count);
}

void OpenGLGraphics::SetViewport(const Rectf& viewport, const Rangef& depthRange)
{
	glViewport(viewport.topLeft.x, viewport.topLeft.y - viewport.size.y, viewport.size.x, viewport.size.y);
	glDepthRange(depthRange.start, depthRange.start + depthRange.length);
}

void OpenGLGraphics::BindRenderTargets(int count, RenderTarget *const* renderTargets, const DepthBuffer* depthBuffer)
{

}