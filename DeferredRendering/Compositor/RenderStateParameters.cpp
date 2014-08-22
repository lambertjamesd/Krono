#include "RenderStateParameters.h"

#include <cstring>

namespace krono
{

RenderStateParameters::ParameterCount::ParameterCount()
{
	memset(this, 0, sizeof(ParameterCount));
}

RenderStateParameters::RenderStateParameters(void)
{

}

RenderStateParameters::~RenderStateParameters(void)
{

}

void RenderStateParameters::AddTexture(const Texture::Ptr& texture, ShaderStage::Type stage)
{
	mTextures[stage].push_back(texture);
	++mParameterCount.textureCount[stage];
}

void RenderStateParameters::AddConstantBuffer(const ConstantBuffer::Ptr& buffer, ShaderStage::Type stage)
{
	mUniforms[stage].push_back(buffer);
	++mParameterCount.uniformCount[stage];
}

const Texture::Ptr& RenderStateParameters::GetTexture(ShaderStage::Type stage, size_t index) const
{
	return mTextures[stage][index];
}

const ConstantBuffer::Ptr& RenderStateParameters::GetConstantBuffer(ShaderStage::Type stage, size_t index) const
{
	return mUniforms[stage][index];
}

const RenderStateParameters::ParameterCount& RenderStateParameters::GetParameterCount() const
{
	return mParameterCount;
}

}
