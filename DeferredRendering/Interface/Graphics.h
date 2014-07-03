#pragma once
#include "VertexBuffer.h"
#include "Shader.h"
#include "Window.h"
#include "RenderTarget.h"
#include "..\Core\Memory.h"
#include "DepthBuffer.h"
#include "Color.h"
#include "..\Math\Rect.h"
#include "..\Math\Range.h"
#include "Texture.h"
#include "IndexBuffer.h"

class Graphics
{
public:
	enum API
	{
		DirectX11,
		OpenGL
	};

	static Auto<Graphics> CreateGraphics(API api);

	virtual ~Graphics(void);

	virtual Auto<IndexBuffer> CreateIndexBuffer(IndexBuffer::Format format) = 0;
	virtual Auto<VertexBuffer> CreateVertexBuffer(const InputLayout& inputLayout) = 0;
	virtual Auto<VertexShader> CreateVertexShader(const std::string& source) = 0;
	virtual Auto<FragmentShader> CreateFragmentShader(const std::string& source) = 0;

	virtual Auto<WindowRenderTarget> CreateWindowRenderTarget(Window& targetWindow) = 0;
	virtual Auto<RenderTarget> CreateOffscreenRenderTarget(Vector2i size, DataFormat format) = 0;

	virtual void Draw(size_t count, size_t offset) = 0;

	virtual void SetViewport(const Rectf& viewport, const Rangef& depthRange) = 0;

	virtual void SetRenderTargets(std::vector<Auto<RenderTarget> > &renderTargets, Auto<DepthBuffer> &depthBuffer) = 0;

	virtual void SetTexture(Auto<Texture> &value, size_t slot) = 0;

	virtual void SetVertexShader(Auto<VertexShader> &vertexShader) = 0;
	virtual void SetFragmentShader(Auto<FragmentShader> &fragmentShader) = 0;
	virtual void SetVertexBuffer(Auto<VertexBuffer> &vertexBuffer) = 0;
	virtual void SetIndexBuffer(Auto<IndexBuffer> &indexBuffer) = 0;

	static GLenum GetGLType(DataFormat::Type type);

protected:
	Graphics(void);
private:

	static GLenum gGLTypeMapping[];
};

