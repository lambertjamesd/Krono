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

namespace krono
{

namespace Topology
{
	enum Type
	{
		Triangles
	};
};

class Graphics
{
public:
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

	virtual void Draw(size_t count, size_t offset) = 0;
	virtual void DrawIndexed(size_t count, size_t offset) = 0;

	virtual void SetViewport(const Rectf& viewport, const Rangef& depthRange) = 0;

	virtual void SetRenderTargets(std::vector<Auto<RenderTarget> > &renderTargets, Auto<DepthBuffer> &depthBuffer) = 0;

	virtual void SetTexture(Auto<Texture> value, size_t slot, ShaderStage::Type stage) = 0;
	
	virtual void SetSampler(Auto<Sampler> value, size_t slot, ShaderStage::Type stage) = 0;

	virtual void SetVertexShader(Auto<VertexShader> &vertexShader) = 0;
	virtual void SetPixelShader(Auto<PixelShader> &fragmentShader) = 0;
	virtual void SetIndexBuffer(Auto<IndexBuffer> &indexBuffer) = 0;
	virtual void SetVertexBuffer(Auto<VertexBuffer> &vertexBuffer) = 0;
	virtual void SetConstantBuffer(Auto<ConstantBuffer> &constantBuffer, size_t slot) = 0;

	virtual bool FlipImageOriginY() const = 0;

	virtual ShaderLanguage ExpectedShaderLanguage() const = 0;

protected:
	Graphics(void);
private:
};

}

