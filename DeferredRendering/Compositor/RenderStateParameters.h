#pragma once

#include "Interface/Shader.h"
#include "Interface/Texture.h"
#include "Interface/ConstantBuffer.h"

#include <vector>

namespace krono
{

class RenderStateParameters
{
public:

	RenderStateParameters(void);
	~RenderStateParameters(void);

	void AddTexture(const Texture::Ptr& texture, ShaderStage::Type stage);
	void AddConstantBuffer(const ConstantBuffer::Ptr& buffer, ShaderStage::Type stage);

	size_t GetTextureCount(ShaderStage::Type stage) const;
	const Texture::Ptr& GetTexture(ShaderStage::Type stage, size_t index) const;
	size_t GetConstantBufferCount(ShaderStage::Type stage) const;
	const ConstantBuffer::Ptr& GetConstantBuffer(ShaderStage::Type stage, size_t index) const;
private:
	std::vector<Texture::Ptr> mTextures[ShaderStage::TypeCount];
	std::vector<ConstantBuffer::Ptr> mUniforms[ShaderStage::TypeCount];
};

}