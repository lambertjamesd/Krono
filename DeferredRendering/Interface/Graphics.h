#pragma once
#include "VertexBuffer.h"
#include "Shader.h"
#include "Window.h"
#include "RenderTarget.h"
#include "Core/Memory.h"
#include "DepthBuffer.h"
#include "Color.h"
#include "Math/Rect.h"
#include "Math/Range.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "DepthBuffer.h"
#include "ConstantBuffer.h"
#include "Sampler.h"
#include "BlendState.h"
#include "DepthState.h"

namespace krono
{

class Graphics
{
public:
	typedef Auto<Graphics> Ptr;

	enum ShaderLanguage
	{
		HLSL_5,
		GLSL_4_4,

		LanguageCount
	};

	enum API
	{
		DirectX11,
		OpenGL,

		APICount
	};

	static Auto<Graphics> CreateGraphics(API api);

	virtual ~Graphics(void);

	virtual Auto<IndexBuffer> CreateIndexBuffer(IndexBuffer::Format format) = 0;
	virtual Auto<VertexBuffer> CreateVertexBuffer(const InputLayout& inputLayout) = 0;
	virtual Auto<ConstantBuffer> CreateConstantBuffer(const ConstantBufferLayout& layout) = 0;
	virtual Auto<VertexShader> CreateVertexShader(const std::string& source) = 0;
	virtual Auto<PixelShader> CreatePixelShader(const std::string& source) = 0;

	virtual Auto<WindowRenderTarget> CreateWindowRenderTarget(Window& targetWindow) = 0;
	virtual Auto<RenderTarget> CreateOffscreenRenderTarget(Vector2i size, DataFormat format) = 0;
	virtual Auto<DepthBuffer> CreateDepthBuffer(Vector2i size, DataFormat::Type format) = 0;

	virtual Auto<Texture2D> CreateTexture2D(Vector2i size, DataFormat format) = 0;

	virtual Auto<Sampler> CreateSampler(const SamplerDescription& description) = 0;

	virtual Auto<BlendState> CreateBlendState(const BlendStateDescription& description) = 0;
	virtual Auto<DepthState> CreateDepthState(const DepthStateDescription& description) = 0;

	virtual void Draw(size_t count, size_t offset) = 0;
	virtual void DrawIndexed(size_t count, size_t offset) = 0;

	virtual void SetViewport(const Rectf& viewport, const Rangef& depthRange) = 0;

	virtual void SetRenderTargets(const std::vector<Auto<RenderTarget> > &renderTargets, const Auto<DepthBuffer> &depthBuffer) = 0;

	virtual void SetTexture(const Auto<Texture>& value, size_t slot, ShaderStage::Type stage) = 0;
	
	virtual void SetSampler(const Auto<Sampler>& value, size_t slot, ShaderStage::Type stage) = 0;

	virtual void SetVertexShader(const Auto<VertexShader> &vertexShader) = 0;
	virtual void SetPixelShader(const Auto<PixelShader> &fragmentShader) = 0;
	virtual void SetIndexBuffer(const Auto<IndexBuffer> &indexBuffer) = 0;
	virtual void SetVertexBuffer(const Auto<VertexBuffer> &vertexBuffer) = 0;
	virtual void SetConstantBuffer(const Auto<ConstantBuffer> &constantBuffer, size_t slot, ShaderStage::Type stage) = 0;

	virtual void SetBlendState(const Auto<BlendState> &blendState) = 0;
	virtual void SetDepthState(const Auto<DepthState> &depthState, UInt32 stencilReference) = 0;

	virtual void SetTopology(Topology::Type topology) = 0;

	virtual bool FlipImageOriginY() const = 0;

	virtual ShaderLanguage ExpectedShaderLanguage() const = 0;

protected:
	Graphics(void);
private:
};

}

