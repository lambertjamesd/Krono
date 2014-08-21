#pragma once

#include <vector>
#include "Core/Hash.h"
#include "Core/Memory.h"
#include "Interface/Shader.h"
#include "OpenGLTexture.h"
#include "OpenGLSampler.h"

namespace krono
{

class OpenGLTextureStorage;

class OpenGLTextureMapping
{
public:
	void AddTextureUnit(ShaderStage::Type stage, size_t textureIndex, size_t samplerIndex);

	UInt32 GetMappingID() const;
private:
	friend class OpenGLTextureStorage;

	struct Element
	{
		Element(ShaderStage::Type stage, size_t textureIndex, size_t samplerIndex);

		ShaderStage::Type stage;
		size_t textureIndex;
		size_t samplerIndex;
		
		bool operator==(const Element& other) const;
	};

	bool operator==(const OpenGLTextureMapping& other) const;

	std::vector<Element> mElements;
	HashUInt32 mHash;
};

class OpenGLTextureStorage
{
public:
	OpenGLTextureStorage();

	void SetTexture(Auto<OpenGLTexture> value, size_t slot, ShaderStage::Type stage);
	void SetSampler(Auto<OpenGLSampler> value, size_t slot, ShaderStage::Type stage);

	void UseMapping(const OpenGLTextureMapping& mapping);

	void UpdateBindings();
private:
	const OpenGLTexture* GetTexture(size_t slot, ShaderStage::Type stage);
	const OpenGLSampler* GetSampler(size_t slot, ShaderStage::Type stage);

	std::vector<Auto<OpenGLTexture> > mTextureBindings[ShaderStage::TypeCount];
	std::vector<Auto<OpenGLSampler> > mSamplerBindings[ShaderStage::TypeCount];

	OpenGLTextureMapping mCurrentMapping;

	bool mIsDirty;
};

}

