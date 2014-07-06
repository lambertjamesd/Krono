#pragma once

#include "..\Interface\Graphics.h"
#include "OpenGLShaderDatabase.h"
#include "OpenGLFBODatabase.h"

class OpenGLVertexBuffer;
class OpenGLVertexShader;
class OpenGLFragmentShader;
class OpenGLIndexBuffer;

class OpenGLGraphics : public Graphics
{
public:
	virtual ~OpenGLGraphics(void);
	
	virtual Auto<IndexBuffer> CreateIndexBuffer(IndexBuffer::Format format);
	virtual Auto<VertexBuffer> CreateVertexBuffer(const InputLayout& inputLayout);
	virtual Auto<VertexShader> CreateVertexShader(const std::string& source);
	virtual Auto<FragmentShader> CreateFragmentShader(const std::string& source);

	virtual Auto<WindowRenderTarget> CreateWindowRenderTarget(Window& targetWindow);
	virtual Auto<RenderTarget> CreateOffscreenRenderTarget(Vector2i size, DataFormat format);

	virtual void Draw(size_t count, size_t offset);
	virtual void DrawIndexed(size_t count, size_t offset);

	virtual void SetViewport(const Rectf& viewport, const Rangef& depthRange);

	virtual void SetRenderTargets(std::vector<Auto<RenderTarget> > &renderTargets, Auto<DepthBuffer> &depthBuffer);

	virtual void SetTexture(Auto<Texture> &value, size_t slot);

	virtual void SetVertexShader(Auto<VertexShader> &vertexShader);
	virtual void SetFragmentShader(Auto<FragmentShader> &fragmentShader);
	virtual void SetVertexBuffer(Auto<VertexBuffer> &vertexBuffer);
	virtual void SetIndexBuffer(Auto<IndexBuffer> &indexBuffer);
	
	static GLenum GetGLType(DataFormat::Type type);
private:
	OpenGLGraphics(void);
	
	void UpdatePendingChanges();

	friend class Graphics;

	static GLenum gGLTypeMapping[];

	OpenGLShaderDatabase mShaderDatabase;
	OpenGLFBODatabase mFBODatabase;

	bool mHasGlewInitialized;

	Auto<OpenGLVertexShader> mCurrentVertexShader;
	Auto<OpenGLFragmentShader> mCurrentFragmentShader;
	Auto<OpenGLShaderProgram> mCurrentShaderProgram;

	Auto<OpenGLVertexBuffer> mCurrentVertexBuffer;
	Auto<OpenGLIndexBuffer> mCurrentIndexBuffer;

	bool mNeedNewProgram;
	
	bool mNeedVertexRebind;
};

