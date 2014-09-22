#pragma once

#include "Interface/Shader.h"
#include "Interface/Texture.h"
#include "Interface/Sampler.h"
#include "Interface/ConstantBuffer.h"
#include "Interface/Shader.h"
#include "Interface/BlendState.h"
#include "Interface/DepthState.h"
#include "Interface/RasterizerState.h"
#include "Interface/MappedConstantBuffer.h"

#include <vector>

namespace krono
{

class RenderState;

class RenderStateParameters
{
public:

	RenderStateParameters(void);
	~RenderStateParameters(void);
	
	void SetTexture(const Texture::Ptr& texture, size_t index, ShaderStage::Type stage);
	void SetSampler(const Sampler::Ptr& sampler, size_t index, ShaderStage::Type stage);
	void SetConstantBuffer(const MappedConstantBuffer::Ptr& buffer, size_t index, ShaderStage::Type stage);

	void AddTexture(const Texture::Ptr& texture, ShaderStage::Type stage);
	void AddSampler(const Sampler::Ptr& texture, ShaderStage::Type stage);
	void AddConstantBuffer(const MappedConstantBuffer::Ptr& buffer, ShaderStage::Type stage);

	size_t GetTextureCount(ShaderStage::Type stage) const;
	const Texture::Ptr& GetTexture(ShaderStage::Type stage, size_t index) const;
	
	size_t GetSamplerCount(ShaderStage::Type stage) const;
	const Sampler::Ptr& GetSampler(ShaderStage::Type stage, size_t index) const;

	size_t GetConstantBufferCount(ShaderStage::Type stage) const;
	const MappedConstantBuffer::Ptr& GetConstantBuffer(ShaderStage::Type stage, size_t index) const;

	void SetVertexShader(const VertexShader::Ptr& vertexShader);
	void SetPixelShader(const PixelShader::Ptr& pixelShader);

	void SetBlendState(const BlendState::Ptr& blendState);
	void SetDepthState(const DepthState::Ptr& depthState);
	void SetStencilReference(UInt32 reference);
	void SetRasterizerState(const RasterizerState::Ptr& rasterizerState);

	template <typename T>
	void SetVariable(const std::string& name, const T& value)
	{
		mMappedVariables.SetValue<T>(name, value);
	}

	template <typename T>
	void SetArrayVariable(const std::string& name, const T* value, size_t count)
	{
		mMappedVariables.SetValue<T>(name, value, count);
	}

	const MappedConstantBufferRelay& GetMappedVariables() const;
private:
	friend class RenderState;

	std::vector<Texture::Ptr> mTextures[ShaderStage::TypeCount];
	std::vector<Sampler::Ptr> mSamplers[ShaderStage::TypeCount];
	std::vector<MappedConstantBuffer::Ptr> mUniforms[ShaderStage::TypeCount];

	VertexShader::Ptr mVertexShader;
	PixelShader::Ptr mPixelShader;

	BlendState::Ptr mBlendState;
	DepthState::Ptr mDepthState;
	UInt32 mStencilReference;
	RasterizerState::Ptr mRasterizerState;

	MappedConstantBufferRelay mMappedVariables;
};

}
