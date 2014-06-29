#pragma once

#include "..\Interface\Graphics.h"

class OpenGLGraphics : public Graphics
{
public:
	OpenGLGraphics(void);
	virtual ~OpenGLGraphics(void);

	virtual Auto<VertexBuffer> CreateVertexBuffer(const InputLayout& inputLayout);
	virtual Auto<VertexShader> CreateVertexShader(const std::string& source);
	virtual Auto<FragmentShader> CreateFragmentShader(const std::string& source);
	virtual Auto<ShaderProgram> CreateShaderProgram(const Auto<VertexShader>& vertexShader, const Auto<FragmentShader>& fragmentShader);

	virtual Auto<WindowRenderTarget> CreateWindowRenderTarget(Window& targetWindow);

	virtual void Draw(size_t count, size_t offset);
	
	virtual void SetViewport(const Rectf& viewport, const Rangef& depthRange);
	virtual void BindRenderTargets(int count, RenderTarget *const* renderTargets, const DepthBuffer* depthBuffer);
private:

	bool mHasGlewInitialized;
};

