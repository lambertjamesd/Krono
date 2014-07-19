#pragma once

#include "..\Interface\Graphics.h"
#include "OpenGLShaderDatabase.h"
#include "OpenGLFBODatabase.h"

class OpenGLVertexBuffer;
class OpenGLVertexShader;
class OpenGLPixelShader;
class OpenGLIndexBuffer;

class OpenGLGraphics : public Graphics
{
public:
	virtual ~OpenGLGraphics(void);
	
	virtual Auto<IndexBuffer> CreateIndexBuffer(IndexBuffer::Format format);
	virtual Auto<VertexBuffer> CreateVertexBuffer(const InputLayout& inputLayout);
	virtual Auto<ConstantBuffer> CreateConstantBuffer(const ConstantBufferLayout& layout);
	virtual Auto<VertexShader> CreateVertexShader(const std::string& source);
	virtual Auto<PixelShader> CreatePixelShader(const std::string& source);

	virtual Auto<WindowRenderTarget> CreateWindowRenderTarget(Window& targetWindow);
	virtual Auto<RenderTarget> CreateOffscreenRenderTarget(Vector2i size, DataFormat format);
	virtual Auto<DepthBuffer> CreateDepthBuffer(Vector2i size, DataFormat::Type format);

	virtual void Draw(size_t count, size_t offset);
	virtual void DrawIndexed(size_t count, size_t offset);

	virtual void SetViewport(const Rectf& viewport, const Rangef& depthRange);

	virtual void SetRenderTargets(std::vector<Auto<RenderTarget> > &renderTargets, Auto<DepthBuffer> &depthBuffer);

	virtual void SetTexture(Auto<Texture> &value, size_t slot);

	virtual void SetVertexShader(Auto<VertexShader> &vertexShader);
	virtual void SetPixelShader(Auto<PixelShader> &fragmentShader);
	virtual void SetIndexBuffer(Auto<IndexBuffer> &indexBuffer);
	virtual void SetVertexBuffer(Auto<VertexBuffer> &vertexBuffer);
	virtual void SetConstantBuffer(Auto<ConstantBuffer> &constantBuffer, size_t slot);
	
	virtual bool FlipImageOriginY() const;
	
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
	Auto<OpenGLPixelShader> mCurrentPixelShader;
	Auto<OpenGLShaderProgram> mCurrentShaderProgram;

	Auto<OpenGLVertexBuffer> mCurrentVertexBuffer;
	Auto<OpenGLIndexBuffer> mCurrentIndexBuffer;

	Vector2i mRenderBufferSize;
	Rectf mViewport;

	bool mNeedNewProgram;
	
	bool mNeedVertexRebind;

	bool mNeedViewportUpdate;
};

