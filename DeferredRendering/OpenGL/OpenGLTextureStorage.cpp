#include "OpenGLTextureStorage.h"
#include <algorithm>
#include <cassert>

void OpenGLTextureMapping::AddTextureUnit(ShaderStage::Type stage, size_t textureIndex, size_t samplerIndex)
{
	mElements.push_back(Element(stage, textureIndex, samplerIndex));
	mHash.Update<Element>(mElements.back());
}

UInt32 OpenGLTextureMapping::GetMappingID() const
{
	return mHash.GetDigest();
}

OpenGLTextureMapping::Element::Element(ShaderStage::Type stage, size_t textureIndex, size_t samplerIndex) :
	stage(stage),
	textureIndex(textureIndex),
	samplerIndex(samplerIndex)
{

}


bool OpenGLTextureMapping::Element::operator==(const Element& other) const
{
	return stage == other.stage && 
		textureIndex == other.textureIndex && 
		samplerIndex == other.samplerIndex;
}

bool OpenGLTextureMapping::operator==(const OpenGLTextureMapping& other) const
{
	return mElements.size() == other.mElements.size() && std::equal(mElements.cbegin(), mElements.cend(), other.mElements.cbegin());
}

OpenGLTextureStorage::OpenGLTextureStorage() :
	mIsDirty(false)
{

}

void OpenGLTextureStorage::SetTexture(Auto<OpenGLTexture> value, size_t slot, ShaderStage::Type stage)
{
	assert(stage < ShaderStage::TypeCount);

	if (slot >= mTextureBindings[stage].size())
	{
		mTextureBindings[stage].resize(slot + 1);
		mIsDirty = true;
	}

	if (mTextureBindings[stage][slot] != value)
	{
		mTextureBindings[stage][slot] = value;
		mIsDirty = true;
	}
}

void OpenGLTextureStorage::SetSampler(Auto<OpenGLSampler> value, size_t slot, ShaderStage::Type stage)
{
	assert(stage < ShaderStage::TypeCount);

	if (slot >= mSamplerBindings[stage].size())
	{
		mSamplerBindings[stage].resize(slot + 1);
		mIsDirty = true;
	}

	if (mSamplerBindings[stage][slot] != value)
	{
		mSamplerBindings[stage][slot] = value;
		mIsDirty = true;
	}
}

void OpenGLTextureStorage::UseMapping(const OpenGLTextureMapping& mapping)
{
	if (mapping.GetMappingID() != mCurrentMapping.GetMappingID())
	{
		mCurrentMapping = mapping;
		mIsDirty = true;
	}
	else
	{
		assert(mapping == mCurrentMapping && "Digests match but binding layouts aren't the same");
	}
}

void OpenGLTextureStorage::UpdateBindings()
{
	if (mIsDirty)
	{
		for (size_t i = 0; i < mCurrentMapping.mElements.size(); ++i)
		{
			OpenGLTextureMapping::Element& currentElement = mCurrentMapping.mElements[i];

			glActiveTexture(GL_TEXTURE0 + i);

			const OpenGLTexture* texture = GetTexture(currentElement.textureIndex, currentElement.stage);

			if (texture != NULL)
			{
				glBindTexture(texture->GetTextureType(), texture->GetGLTexture());
			}

			const OpenGLSampler* sampler = GetSampler(currentElement.samplerIndex, currentElement.stage);

			if (sampler != NULL)
			{
				glBindSampler(i, sampler->GetGLSampler());
			}
			else
			{
				glBindSampler(i, 0);
			}
		}

		mIsDirty = false;
	}
}

const OpenGLTexture* OpenGLTextureStorage::GetTexture(size_t slot, ShaderStage::Type stage)
{
	assert(stage < ShaderStage::TypeCount);

	if (slot < mTextureBindings[stage].size())
	{
		return mTextureBindings[stage][slot].get();
	}
	else
	{
		return NULL;
	}
}

const OpenGLSampler* OpenGLTextureStorage::GetSampler(size_t slot, ShaderStage::Type stage)
{
	assert(stage < ShaderStage::TypeCount);

	if (slot < mSamplerBindings[stage].size())
	{
		return mSamplerBindings[stage][slot].get();
	}
	else
	{
		return NULL;
	}
}