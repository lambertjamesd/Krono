#pragma once
#include "VertexBuffer.h"
#include "ShaderProgram.h"
#include "Window.h"
#include "RenderTarget.h"
#include "..\Core\Memory.h"
#include "DepthBuffer.h"
#include "Color.h"
#include "..\Math\Rect.h"
#include "..\Math\Range.h"

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

	virtual Auto<VertexBuffer> CreateVertexBuffer(const InputLayout& inputLayout) = 0;
	virtual Auto<VertexShader> CreateVertexShader(const std::string& source) = 0;
	virtual Auto<FragmentShader> CreateFragmentShader(const std::string& source) = 0;
	virtual Auto<ShaderProgram> CreateShaderProgram(const Auto<VertexShader>& vertexShader, const Auto<FragmentShader>& fragmentShader) = 0;

	virtual Auto<WindowRenderTarget> CreateWindowRenderTarget(Window& targetWindow) = 0;

	virtual void Draw(size_t count, size_t offset) = 0;

	virtual void SetViewport(const Rectf& viewport, const Rangef& depthRange) = 0;
	virtual void BindRenderTargets(int count, RenderTarget *const* renderTargets, const DepthBuffer* depthBuffer) = 0;

protected:
	Graphics(void);
private:
};

