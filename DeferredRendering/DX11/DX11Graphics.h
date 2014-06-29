#pragma once
#include "..\Interface\Graphics.h"

class DX11Graphics : public Graphics
{
public:
	~DX11Graphics(void);
	
	virtual Auto<VertexBuffer> CreateVertexBuffer(const InputLayout& inputLayout);
	virtual Auto<VertexShader> CreateVertexShader(const std::string& source);
	virtual Auto<FragmentShader> CreateFragmentShader(const std::string& source);
	virtual Auto<ShaderProgram> CreateShaderProgram(const Auto<VertexShader>& vertexShader, const Auto<FragmentShader>& fragmentShader);
	
	virtual Auto<WindowRenderTarget> CreateWindowRenderTarget(Window& targetWindow);
	
	virtual void Draw(size_t count, size_t offset);
	
	virtual void SetViewport(const Rectf& viewport, const Rangef& depthRange);
	virtual void BindRenderTargets(int count, RenderTarget *const* enderTargets, const DepthBuffer* depthBuffer);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	IDXGIFactory* GetDXGIFactory();
private:
	DX11Graphics(void);

	friend class Graphics;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	IDXGIFactory *mDXGIFactory;
	D3D_FEATURE_LEVEL mFeatureLevel;
};

