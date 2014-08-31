#pragma once

#include "Interface/Shader.h"
#include "Interface/Texture.h"
#include "Interface/Sampler.h"
#include "Interface/ConstantBuffer.h"
#include "Interface/Shader.h"

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
	void SetConstantBuffer(const ConstantBuffer::Ptr& buffer, size_t index, ShaderStage::Type stage);

	void AddTexture(const Texture::Ptr& texture, ShaderStage::Type stage);
	void AddSampler(const Sampler::Ptr& texture, ShaderStage::Type stage);
	void AddConstantBuffer(const ConstantBuffer::Ptr& buffer, ShaderStage::Type stage);

	size_t GetTextureCount(ShaderStage::Type stage) const;
	const Texture::Ptr& GetTexture(ShaderStage::Type stage, size_t index) const;
	
	size_t GetSamplerCount(ShaderStage::Type stage) const;
	const Sampler::Ptr& GetSampler(ShaderStage::Type stage, size_t index) const;

	size_t GetConstantBufferCount(ShaderStage::Type stage) const;
	const ConstantBuffer::Ptr& GetConstantBuffer(ShaderStage::Type stage, size_t index) const;

	void SetVertexShader(const VertexShader::Ptr& vertexShader);
	void SetPixelShader(const PixelShader::Ptr& pixelShader);
private:
	friend class RenderState;

	std::vector<Texture::Ptr> mTextures[ShaderStage::TypeCount];
	std::vector<Sampler::Ptr> mSamplers[ShaderStage::TypeCount];
	std::vector<ConstantBuffer::Ptr> mUniforms[ShaderStage::TypeCount];

	VertexShader::Ptr mVertexShader;
	PixelShader::Ptr mPixelShader;
};

}
