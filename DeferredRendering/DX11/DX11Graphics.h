#pragma once
#include "Interface\Graphics.h"

#include "DX11Common.h"

class DX11VertexBuffer;
class DX11VertexShader;

class DX11Graphics : public Graphics
{
public:
	~DX11Graphics(void);
	
	virtual Auto<IndexBuffer> CreateIndexBuffer(IndexBuffer::Format format);
	virtual Auto<VertexBuffer> CreateVertexBuffer(const InputLayout& inputLayout);
	virtual Auto<ConstantBuffer> CreateConstantBuffer(const ConstantBufferLayout& layout);
	virtual Auto<VertexShader> CreateVertexShader(const std::string& source);
	virtual Auto<PixelShader> CreatePixelShader(const std::string& source);

	virtual Auto<WindowRenderTarget> CreateWindowRenderTarget(Window& targetWindow);
	virtual Auto<RenderTarget> CreateOffscreenRenderTarget(Vector2i size, DataFormat format);
	virtual Auto<DepthBuffer> CreateDepthBuffer(Vector2i size, DataFormat::Type format);
	
	virtual Auto<Texture2D> CreateTexture2D(Vector2i size, DataFormat format);
	
	virtual void Draw(size_t count, size_t offset);
	virtual void DrawIndexed(size_t count, size_t offset);
	
	virtual void SetViewport(const Rectf& viewport, const Rangef& depthRange);

	virtual void SetRenderTargets(std::vector<Auto<RenderTarget> > &renderTargets, Auto<DepthBuffer> &depthBuffer);

	virtual void SetTexture(Auto<Texture> value, size_t slot);

	virtual void SetVertexShader(Auto<VertexShader> &vertexShader);
	virtual void SetPixelShader(Auto<PixelShader> &fragmentShader);
	virtual void SetIndexBuffer(Auto<IndexBuffer> &indexBuffer);
	virtual void SetVertexBuffer(Auto<VertexBuffer> &vertexBuffer);
	virtual void SetConstantBuffer(Auto<ConstantBuffer> &constantBuffer, size_t slot);
	
	virtual bool FlipImageOriginY() const;

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	IDXGIFactory* GetDXGIFactory();

	static DXGI_FORMAT GetDXFormat(const DataFormat& dataFormat);
private:
	DX11Graphics(void);

	void UpdatePendingChanges();

	friend class Graphics;
	
	static DXGI_FORMAT gFormatMapping[][4];

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	IDXGIFactory *mDXGIFactory;
	D3D_FEATURE_LEVEL mFeatureLevel;

	Auto<DX11VertexShader> mCurrentVertexShader;
	Auto<DX11VertexBuffer> mCurrentVertexBuffer;

	bool mNeedNewInputMapping;
	bool mHasIndexBuffer;
};

