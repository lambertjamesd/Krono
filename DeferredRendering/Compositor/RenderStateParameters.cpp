#include "RenderStateParameters.h"

#include <cstring>

namespace krono
{

RenderStateParameters::RenderStateParameters(void)
{

}

RenderStateParameters::~RenderStateParameters(void)
{

}

size_t RenderStateParameters::GetTextureCount(ShaderStage::Type stage) const
{
	return mTextures[stage].size();
}

void RenderStateParameters::AddTexture(const Texture::Ptr& texture, ShaderStage::Type stage)
{
	mTextures[stage].push_back(texture);
}

size_t RenderStateParameters::GetConstantBufferCount(ShaderStage::Type stage) const
{
	return mUniforms[stage].size();
}

void RenderStateParameters::AddConstantBuffer(const ConstantBuffer::Ptr& buffer, ShaderStage::Type stage)
{
	mUniforms[stage].push_back(buffer);
}

const Texture::Ptr& RenderStateParameters::GetTexture(ShaderStage::Type stage, size_t index) const
{
	return mTextures[stage][index];
}

const ConstantBuffer::Ptr& RenderStateParameters::GetConstantBuffer(ShaderStage::Type stage, size_t index) const
{
	return mUniforms[stage][index];
}

}
