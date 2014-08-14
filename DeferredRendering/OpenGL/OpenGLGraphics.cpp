
#include "OpenGLGraphics.h"
#include "OpenGLShader.h"
#include "OpenGLShaderProgram.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLConstantBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLOffscreenRenderTarget.h"
#include "OpenGLWindowRenderTarget.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLTexture2D.h"
#include "OpenGLDepthBuffer.h"
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

Auto<ConstantBuffer> OpenGLGraphics::CreateConstantBuffer(const ConstantBufferLayout& layout)
{
	return Auto<ConstantBuffer>(new OpenGLConstantBuffer(layout));
}


Auto<VertexShader> OpenGLGraphics::CreateVertexShader(const std::string& source)
{
	return Auto<VertexShader>(new OpenGLVertexShader(source));
}

Auto<PixelShader> OpenGLGraphics::CreatePixelShader(const std::string& source)
{
	return Auto<PixelShader>(new OpenGLPixelShader(source));
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

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		mHasGlewInitialized = true;
	}

	return result;
}


Auto<RenderTarget> OpenGLGraphics::CreateOffscreenRenderTarget(Vector2i size, DataFormat format)
{
	return Auto<RenderTarget>(new OpenGLOffscreenRenderTarget(size, format));
}

Auto<DepthBuffer> OpenGLGraphics::CreateDepthBuffer(Vector2i size, DataFormat::Type format)
{
	return Auto<DepthBuffer>(new OpenGLDepthBuffer(size, format));
}

Auto<Texture2D> OpenGLGraphics::CreateTexture2D(Vector2i size, DataFormat format)
{
	return Auto<Texture2D>(new OpenGLTexture2D(size, format));
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
	size_t byteOffset = offset * mCurrentIndexBuffer->GetStrideSize();
	glDrawElementsBaseVertex(GL_TRIANGLES, count, glFormat, (void*)byteOffset, 0);
}

void OpenGLGraphics::SetViewport(const Rectf& viewport, const Rangef& depthRange)
{
	mViewport = viewport;
	mNeedViewportUpdate = true;
	glDepthRange(depthRange.start, depthRange.end);
}

void OpenGLGraphics::SetRenderTargets(std::vector<Auto<RenderTarget> > &renderTargets, Auto<DepthBuffer> &depthBuffer)
{
	vector<Auto<OpenGLRenderTarget> > renderBuffers;

	for (auto it = renderTargets.begin(); it != renderTargets.end(); ++it)
	{
		renderBuffers.push_back(std::dynamic_pointer_cast<OpenGLRenderTarget>(*it));
	}

	mFBODatabase.GetFrameBuffer(renderBuffers, dynamic_cast<OpenGLDepthBuffer*>(depthBuffer.get())).Use();

	if (depthBuffer == NULL)
	{
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
	}

	if (renderTargets.size() > 0)
	{
		mRenderBufferSize = renderTargets[0]->GetSize();
	}
	else if (depthBuffer != NULL)
	{
		mRenderBufferSize = depthBuffer->GetSize();
	}
	else
	{
		mRenderBufferSize = Vector2i();
	}

	mNeedViewportUpdate = true;
}

void OpenGLGraphics::SetTexture(Auto<Texture> value, size_t slot)
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

void OpenGLGraphics::SetPixelShader(Auto<PixelShader> &fragmentShader)
{
	mCurrentPixelShader = std::dynamic_pointer_cast<OpenGLPixelShader>(fragmentShader);
	mNeedNewProgram = true;
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

void OpenGLGraphics::SetVertexBuffer(Auto<VertexBuffer> &vertexBuffer)
{
	mCurrentVertexBuffer = std::dynamic_pointer_cast<OpenGLVertexBuffer>(vertexBuffer);
	mCurrentVertexBuffer->Use();
	mNeedVertexRebind = true;
}

void OpenGLGraphics::SetConstantBuffer(Auto<ConstantBuffer> &constantBuffer, size_t slot)
{
	OpenGLConstantBuffer* glBuffer = dynamic_cast<OpenGLConstantBuffer*>(constantBuffer.get());

	if (glBuffer == NULL)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, 0);
	}
	else
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, glBuffer->GetBuffer());
	}
}

bool OpenGLGraphics::FlipImageOriginY() const
{
	return true;
}

GLenum OpenGLGraphics::gGLTypeMapping[] = {GL_FLOAT, GL_UNSIGNED_BYTE, GL_SHORT, 0, GL_FLOAT, 0, 0};

GLenum OpenGLGraphics::GetGLType(DataFormat::Type type)
{
	static_assert((sizeof(gGLTypeMapping) / sizeof(gGLTypeMapping[0])) == DataFormat::TypeCount, "Missing elements in gTypeMapping");
	return gGLTypeMapping[type];
}

void OpenGLGraphics::UpdatePendingChanges()
{
	if (mNeedNewProgram && 
		mCurrentVertexShader != NULL &&
		mCurrentPixelShader != NULL &&
		mCurrentVertexBuffer != NULL)
	{
		mCurrentShaderProgram = mShaderDatabase.GetProgram(*mCurrentVertexShader, *mCurrentPixelShader);
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

	if (mNeedViewportUpdate)
	{
		float bottom = mRenderBufferSize.y - (mViewport.topLeft.y + mViewport.size.y);

		glViewport((GLint)mViewport.topLeft.x, (GLint)bottom, (GLsizei)mViewport.size.x, (GLsizei)mViewport.size.y);
		mNeedViewportUpdate = false;
	}
}