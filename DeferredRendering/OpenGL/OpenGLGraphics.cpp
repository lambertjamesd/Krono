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

Auto<IndexBuffer> OpenGLGraphics::CreateIndexBuffer(IndexBuffer::Format format)
{
	return Auto<IndexBuffer>(NULL);
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


Auto<RenderTarget> OpenGLGraphics::CreateOffscreenRenderTarget(Vector2i size, DataFormat format)
{
	return Auto<WindowRenderTarget>(NULL);
}

void OpenGLGraphics::Draw(size_t count, size_t offset)
{
	glDrawArrays(GL_TRIANGLES, offset, count);
}

void OpenGLGraphics::SetViewport(const Rectf& viewport, const Rangef& depthRange)
{
	glViewport(viewport.topLeft.x, viewport.topLeft.y, viewport.size.x, viewport.size.y);
	glDepthRange(depthRange.start, depthRange.start + depthRange.length);
}

void OpenGLGraphics::SetRenderTargets(std::vector<Auto<RenderTarget> > &renderTargets, Auto<DepthBuffer> &depthBuffer)
{

}

void OpenGLGraphics::SetTexture(Auto<Texture> &value, size_t slot)
{

}

void OpenGLGraphics::SetVertexShader(Auto<VertexShader> &vertexShader)
{

}

void OpenGLGraphics::SetFragmentShader(Auto<FragmentShader> &fragmentShader)
{

}

void OpenGLGraphics::SetVertexBuffer(Auto<VertexBuffer> &vertexBuffer)
{

}

void OpenGLGraphics::SetIndexBuffer(Auto<IndexBuffer> &indexBuffer)
{

}

GLenum OpenGLGraphics::gGLTypeMapping[] = {GL_FLOAT, GL_UNSIGNED_BYTE};

GLenum OpenGLGraphics::GetGLType(DataFormat::Type type)
{
	static_assert((sizeof(gGLTypeMapping) / sizeof(gGLTypeMapping[0])) == DataFormat::TypeCount, "Missing elements in gTypeMapping");
	return gGLTypeMapping[type];
}