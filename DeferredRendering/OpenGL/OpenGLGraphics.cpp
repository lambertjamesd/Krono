#include "stdafx.h"
#include "OpenGLGraphics.h"
#include "OpenGLShader.h"
#include "OpenGLShaderProgram.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLOffscreenRenderTarget.h"
#include "OpenGLWindowRenderTarget.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLTexture2D.h"
#include <iostream>

using namespace std;

OpenGLGraphics::OpenGLGraphics(void) :
		mHasGlewInitialized(false),
		mNeedNewProgram(false),
		mNeedVertexRebind(false)
{

}


OpenGLGraphics::~OpenGLGraphics(void)
{
}

Auto<IndexBuffer> OpenGLGraphics::CreateIndexBuffer(IndexBuffer::Format format)
{
	return Auto<IndexBuffer>(new OpenGLIndexBuffer(format));
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
	return Auto<RenderTarget>(new OpenGLOffscreenRenderTarget(size, format));
}

void OpenGLGraphics::Draw(size_t count, size_t offset)
{
	UpdatePendingChanges();
	glDrawArrays(GL_TRIANGLES, offset, count);
}

void OpenGLGraphics::DrawIndexed(size_t count, size_t offset)
{
	UpdatePendingChanges();
	GLenum glFormat = OpenGLIndexBuffer::GetGLFormat(mCurrentIndexBuffer->GetFormat());
	glDrawElements(GL_TRIANGLES, count, glFormat, 0);
}

void OpenGLGraphics::SetViewport(const Rectf& viewport, const Rangef& depthRange)
{
	glViewport(viewport.topLeft.x, viewport.topLeft.y, viewport.size.x, viewport.size.y);
	glDepthRange(depthRange.start, depthRange.start + depthRange.length);
}

void OpenGLGraphics::SetRenderTargets(std::vector<Auto<RenderTarget> > &renderTargets, Auto<DepthBuffer> &depthBuffer)
{
	vector<Auto<OpenGLRenderTarget> > renderBuffers;

	for (auto it = renderTargets.begin(); it != renderTargets.end(); ++it)
	{
		renderBuffers.push_back(std::dynamic_pointer_cast<OpenGLRenderTarget>(*it));
	}

	mFBODatabase.GetFrameBuffer(renderBuffers).Use();
}

void OpenGLGraphics::SetTexture(Auto<Texture> &value, size_t slot)
{
	OpenGLTexture *texture = dynamic_cast<OpenGLTexture*>(value.get());

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(texture->GetTextureType(), texture->GetGLTexture());
}

void OpenGLGraphics::SetVertexShader(Auto<VertexShader> &vertexShader)
{
	mCurrentVertexShader = std::dynamic_pointer_cast<OpenGLVertexShader>(vertexShader);
	mNeedNewProgram = true;
}

void OpenGLGraphics::SetFragmentShader(Auto<FragmentShader> &fragmentShader)
{
	mCurrentFragmentShader = std::dynamic_pointer_cast<OpenGLFragmentShader>(fragmentShader);
	mNeedNewProgram = true;
}

void OpenGLGraphics::SetVertexBuffer(Auto<VertexBuffer> &vertexBuffer)
{
	mCurrentVertexBuffer = std::dynamic_pointer_cast<OpenGLVertexBuffer>(vertexBuffer);
	mCurrentVertexBuffer->Use();
	mNeedVertexRebind = true;
}

void OpenGLGraphics::SetIndexBuffer(Auto<IndexBuffer> &indexBuffer)
{
	mCurrentIndexBuffer = std::dynamic_pointer_cast<OpenGLIndexBuffer>(indexBuffer);

	if (mCurrentIndexBuffer == NULL)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		mCurrentIndexBuffer->Use();
	}
}

GLenum OpenGLGraphics::gGLTypeMapping[] = {GL_FLOAT, GL_UNSIGNED_BYTE};

GLenum OpenGLGraphics::GetGLType(DataFormat::Type type)
{
	static_assert((sizeof(gGLTypeMapping) / sizeof(gGLTypeMapping[0])) == DataFormat::TypeCount, "Missing elements in gTypeMapping");
	return gGLTypeMapping[type];
}

void OpenGLGraphics::UpdatePendingChanges()
{
	if (mNeedNewProgram && 
		mCurrentVertexShader != NULL &&
		mCurrentFragmentShader != NULL &&
		mCurrentVertexBuffer != NULL)
	{
		mCurrentShaderProgram = mShaderDatabase.GetProgram(*mCurrentVertexShader, *mCurrentFragmentShader);
		mCurrentShaderProgram->Use();
		mNeedNewProgram = false;

		mNeedVertexRebind = true;
	}

	if (mNeedVertexRebind &&
		mCurrentShaderProgram != NULL &&
		mCurrentVertexBuffer != NULL)
	{
		mCurrentShaderProgram->BindVertexBuffer(*mCurrentVertexBuffer);
	}
}