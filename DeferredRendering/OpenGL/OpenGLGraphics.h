#pragma once

#include "..\Interface\Graphics.h"

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

	friend class Graphics;

	static GLenum gGLTypeMapping[];

	bool mHasGlewInitialized;
};

