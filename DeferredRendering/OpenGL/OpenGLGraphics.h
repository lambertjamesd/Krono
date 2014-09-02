#pragma once

#include "OpenGLCommon.h"

#include "Interface/Graphics.h"
#include "OpenGLShaderDatabase.h"
#include "OpenGLFBODatabase.h"
#include "OpenGLTextureStorage.h"
#include "OpenGLConstantBufferStorage.h"

namespace krono
{

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
	virtual void SetConstantBuffer(const Auto<ConstantBuffer>& constantBuffer, size_t slot, ShaderStage::Type stage);
	
	virtual void SetBlendState(const Auto<BlendState> &blendState);
	virtual void SetDepthState(const Auto<DepthState> &depthState, UInt32 stencilReference);
	virtual void SetRasterizerState(const Auto<RasterizerState> &rasterizerState);
	
	virtual void SetTopology(Topology::Type topology);

	virtual bool FlipImageOriginY() const;

	virtual Graphics::ShaderLanguage ExpectedShaderLanguage() const;
	
	static GLenum GetGLType(DataFormat::Type type);
private:
	OpenGLGraphics(void);
	
	void UpdatePendingChanges();

	friend class Graphics;

	static GLenum gGLTypeMapping[DataFormat::TypeCount];
	static GLenum gTopologyMapping[Topology::Count];

	OpenGLShaderDatabase mShaderDatabase;
	OpenGLFBODatabase mFBODatabase;
	OpenGLTextureStorage mTextureStorage;
	OpenGLConstantBufferStorage mConstantBufferStorage;

	bool mHasGlewInitialized;

	Auto<OpenGLVertexShader> mCurrentVertexShader;
	Auto<OpenGLPixelShader> mCurrentPixelShader;
	Auto<OpenGLShaderProgram> mCurrentShaderProgram;

	Auto<OpenGLVertexBuffer> mCurrentVertexBuffer;
	Auto<OpenGLIndexBuffer> mCurrentIndexBuffer;

	Vector2i mRenderBufferSize;
	Rectf mViewport;
	GLenum mTopology;

	bool mNeedNewProgram;
	
	bool mNeedVertexRebind;

	bool mNeedViewportUpdate;
};

}

