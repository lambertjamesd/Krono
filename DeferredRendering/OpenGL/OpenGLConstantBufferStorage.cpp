
#include "OpenGLConstantBufferStorage.h"
#include <algorithm>
#include <cassert>

namespace krono
{

void OpenGLConstantBufferMapping::AddConstantBuffer(ShaderStage::Type stage, size_t uniformIndex)
{
	mElements.push_back(Element(stage, uniformIndex));
	mHash.Update<Element>(mElements.back());
}

UInt32 OpenGLConstantBufferMapping::GetMappingID() const
{
	return mHash.GetDigest();
}

OpenGLConstantBufferMapping::Element::Element(ShaderStage::Type stage, size_t uniformIndex) :
	stage(stage),
	uniformIndex(uniformIndex)
{

}

bool OpenGLConstantBufferMapping::Element::operator==(const Element& other) const
{
	return stage == other.stage && 
		uniformIndex == other.uniformIndex;
}

bool OpenGLConstantBufferMapping::operator==(const OpenGLConstantBufferMapping& other) const
{
	return mElements.size() == other.mElements.size() && std::equal(mElements.cbegin(), mElements.cend(), other.mElements.cbegin());
}

////////////////////////////////////////////

OpenGLConstantBufferStorage::OpenGLConstantBufferStorage() :
	mIsDirty(false)
{

}

void OpenGLConstantBufferStorage::SetConstantBuffer(const OpenGLConstantBuffer::Ptr& value, size_t slot, ShaderStage::Type stage)
{
	assert(stage < ShaderStage::TypeCount);

	if (slot >= mConstantBuffers[stage].size())
	{
		mConstantBuffers[stage].resize(slot + 1);
		mIsDirty = true;
	}

	if (mConstantBuffers[stage][slot] != value)
	{
		mConstantBuffers[stage][slot] = value;
		mIsDirty = true;
	}
}

void OpenGLConstantBufferStorage::UseMapping(const OpenGLConstantBufferMapping& mapping)
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

void OpenGLConstantBufferStorage::UpdateBindings()
{
	if (mIsDirty)
	{
		for (size_t slot = 0; slot < mCurrentMapping.mElements.size(); ++slot)
		{
			OpenGLConstantBufferMapping::Element& currentElement = mCurrentMapping.mElements[slot];

			const OpenGLConstantBuffer* buffer = GetConstantBuffer(currentElement.uniformIndex, currentElement.stage);

			if (buffer == NULL)
			{
				glBindBufferBase(GL_UNIFORM_BUFFER, slot, 0);
			}
			else
			{
				glBindBufferBase(GL_UNIFORM_BUFFER, slot, buffer->GetBuffer());
			}
		}

		mIsDirty = false;
	}
}

const OpenGLConstantBuffer* OpenGLConstantBufferStorage::GetConstantBuffer(size_t slot, ShaderStage::Type stage)
{
	assert(stage < ShaderStage::TypeCount);

	if (slot < mConstantBuffers[stage].size())
	{
		return mConstantBuffers[stage][slot].get();
	}
	else
	{
		return NULL;
	}
}

}
