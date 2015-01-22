#pragma once
#include "Interface/Graphics.h"

#include "DX11Common.h"

namespace krono
{

class DX11VertexBuffer;
class DX11VertexShader;

class DX11Graphics : public Graphics
{
public:
	~DX11Graphics(void);
	
	virtual Auto<IndexBuffer> CreateIndexBuffer(IndexBuffer::Format format, BufferAccess::Type bufferAccess);
	virtual Auto<VertexBuffer> CreateVertexBuffer(const InputLayout& inputLayout, BufferAccess::Type bufferAccess);
	virtual Auto<ConstantBuffer> CreateConstantBuffer(const ConstantBufferLayout& layout);
	virtual Auto<VertexShader> CreateVertexShader(const std::string& source);
	virtual Auto<PixelShader> CreatePixelShader(const std::string& source);

	virtual Auto<WindowRenderTarget> CreateWindowRenderTarget(Window& targetWindow);
	virtual Auto<RenderTarget> CreateOffscreenRenderTarget(Vector2i size, DataFormat format);
	virtual Auto<DepthBuffer> CreateDepthBuffer(Vector2i size, DataFormat::Type format);
	
	virtual Auto<Texture2D> CreateTexture2D(Vector2i size, DataFormat format);
	
	virtual Auto<Sampler> CreateSampler(const SamplerDescription& description);

	virtual Auto<BlendState> CreateBlendState(const BlendStateDescription& description);
	virtual Auto<DepthState> CreateDepthState(const DepthStateDescription& description);
	virtual Auto<RasterizerState> CreateRasterizerState(const RasterizerStateDescription& description);
	
	virtual void Draw(size_t count, size_t offset);
	virtual void DrawIndexed(size_t count, size_t offset);
	
	virtual void SetViewport(const Rectf& viewport, const Rangef& depthRange);

	virtual void SetRenderTargets(const std::vector<Auto<RenderTarget> > &renderTargets, const Auto<DepthBuffer> &depthBuffer);

	virtual void SetTexture(const Auto<Texture>& value, size_t slot, ShaderStage::Type stage);

	virtual void SetSampler(const Auto<Sampler>& value, size_t slot, ShaderStage::Type stage);

	virtual void SetVertexShader(const Auto<VertexShader> &vertexShader);
	virtual void SetPixelShader(const Auto<PixelShader> &fragmentShader);
	virtual void SetIndexBuffer(const Auto<IndexBuffer> &indexBuffer);
	virtual void SetVertexBuffer(const Auto<VertexBuffer> &vertexBuffer);
	virtual void SetConstantBuffer(const Auto<ConstantBuffer> &constantBuffer, size_t slot, ShaderStage::Type stage);

	virtual void SetBlendState(const Auto<BlendState> &blendState);
	virtual void SetDepthState(const Auto<DepthState> &depthState, UInt32 stencilReference);
	virtual void SetRasterizerState(const Auto<RasterizerState> &rasterizerState);

	virtual void SetTopology(Topology::Type topology);
	
	virtual bool FlipImageOriginY() const;

	virtual Graphics::ShaderLanguage ExpectedShaderLanguage() const;

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	IDXGIFactory* GetDXGIFactory();

	static DXGI_FORMAT GetDXFormat(const DataFormat& dataFormat);
private:
	DX11Graphics(void);

	void UpdatePendingChanges();

	friend class Graphics;
	
	static DXGI_FORMAT gFormatMapping[DataFormat::TypeCount][4];
	static D3D11_PRIMITIVE_TOPOLOGY gTopologyMapping[Topology::Count];

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	IDXGIFactory *mDXGIFactory;
	D3D_FEATURE_LEVEL mFeatureLevel;

	Auto<DX11VertexShader> mCurrentVertexShader;
	Auto<VertexBuffer> mCurrentVertexBuffer;

	bool mNeedNewInputMapping;
	bool mHasIndexBuffer;
};

}

