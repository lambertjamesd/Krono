#include "RenderStateParameters.h"

#include <cstring>

namespace krono
{

RenderStateParameters::RenderStateParameters(void) :
	mStencilReference(0)
{

}

RenderStateParameters::~RenderStateParameters(void)
{

}

void RenderStateParameters::SetTexture(const Texture::Ptr& texture, size_t index, ShaderStage::Type stage)
{
	if (mTextures[stage].size() < index + 1)
	{
		mTextures[stage].resize(index + 1);
	}

	mTextures[stage][index] = texture;
}

void RenderStateParameters::SetSampler(const Sampler::Ptr& texture, size_t index, ShaderStage::Type stage)
{
	if (mSamplers[stage].size() < index + 1)
	{
		mSamplers[stage].resize(index + 1);
	}

	mSamplers[stage][index] = texture;
}

void RenderStateParameters::SetConstantBuffer(const MappedConstantBuffer::Ptr& buffer, size_t index, ShaderStage::Type stage)
{
	if (mUniforms[stage].size() < index + 1)
	{
		mUniforms[stage].resize(index + 1);
	}

	if (mUniforms[stage][index] != NULL)
	{
		mMappedVariables.RemoveBuffer(mUniforms[stage][index]);
	}
	
	mUniforms[stage][index] = buffer;

	if (buffer != NULL)
	{
		mMappedVariables.AddBuffer(buffer);
	}
}

void RenderStateParameters::AddTexture(const Texture::Ptr& texture, ShaderStage::Type stage)
{
	mTextures[stage].push_back(texture);
}

void RenderStateParameters::AddSampler(const Sampler::Ptr& texture, ShaderStage::Type stage)
{
	mSamplers[stage].push_back(texture);
}

void RenderStateParameters::AddConstantBuffer(const MappedConstantBuffer::Ptr& buffer, ShaderStage::Type stage)
{
	mUniforms[stage].push_back(buffer);

	if (buffer != NULL)
	{
		mMappedVariables.AddBuffer(buffer);
	}
}

size_t RenderStateParameters::GetTextureCount(ShaderStage::Type stage) const
{
	return mTextures[stage].size();
}

const Texture::Ptr& RenderStateParameters::GetTexture(ShaderStage::Type stage, size_t index) const
{
	return mTextures[stage][index];
}

size_t RenderStateParameters::GetSamplerCount(ShaderStage::Type stage) const
{
	return mSamplers[stage].size();
}

const Sampler::Ptr& RenderStateParameters::GetSampler(ShaderStage::Type stage, size_t index) const
{
	return mSamplers[stage][index];
}

size_t RenderStateParameters::GetConstantBufferCount(ShaderStage::Type stage) const
{
	return mUniforms[stage].size();
}

const MappedConstantBuffer::Ptr& RenderStateParameters::GetConstantBuffer(ShaderStage::Type stage, size_t index) const
{
	return mUniforms[stage][index];
}

void RenderStateParameters::SetVertexShader(const VertexShader::Ptr& vertexShader)
{
	mVertexShader = vertexShader;
}

void RenderStateParameters::SetPixelShader(const PixelShader::Ptr& pixelShader)
{
	mPixelShader = pixelShader;
}

void RenderStateParameters::SetBlendState(const BlendState::Ptr& blendState)
{
	mBlendState = blendState;
}

void RenderStateParameters::SetDepthState(const DepthState::Ptr& depthState)
{
	mDepthState = depthState;
}

void RenderStateParameters::SetStencilReference(UInt32 reference)
{
	mStencilReference = reference;
}

void RenderStateParameters::SetRasterizerState(const RasterizerState::Ptr& rasterizerState)
{
	mRasterizerState = rasterizerState;
}

const MappedConstantBufferRelay& RenderStateParameters::GetMappedVariables() const
{
	return mMappedVariables;
}

}
