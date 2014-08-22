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
	struct ParameterCount
	{
		ParameterCount();

		size_t textureCount[ShaderStage::TypeCount];
		size_t uniformCount[ShaderStage::TypeCount];
	};

	RenderStateParameters(void);
	~RenderStateParameters(void);

	void AddTexture(const Texture::Ptr& texture, ShaderStage::Type stage);
	void AddConstantBuffer(const ConstantBuffer::Ptr& buffer, ShaderStage::Type stage);

	const Texture::Ptr& GetTexture(ShaderStage::Type stage, size_t index) const;
	const ConstantBuffer::Ptr& GetConstantBuffer(ShaderStage::Type stage, size_t index) const;

	const ParameterCount& GetParameterCount() const;
private:
	std::vector<Texture::Ptr> mTextures[ShaderStage::TypeCount];
	std::vector<ConstantBuffer::Ptr> mUniforms[ShaderStage::TypeCount];

	ParameterCount mParameterCount;
};

}