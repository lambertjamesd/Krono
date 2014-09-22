#include "MappedConstantBuffer.h"
#include <cassert>
#include <algorithm>
#include <cstring>

using namespace std;

namespace krono
{

MappedConstantBuffer::MappedConstantBuffer(const ConstantBuffer::Ptr& constantBuffer, const ShaderUniformBlockLayout& blockLayout) :
	mConstantBuffer(constantBuffer),
	mBlockLayout(blockLayout),
	mBuffer(blockLayout.GetSize()),
	mIsDirty(true)
{
}


MappedConstantBuffer::MappedConstantBuffer(const ConstantBuffer::Ptr& constantBuffer, const ShaderUniformBlockLayout& blockLayout, size_t extraBytes):
	mConstantBuffer(constantBuffer),
	mBlockLayout(blockLayout),
	mBuffer(blockLayout.GetSize() + extraBytes),
	mIsDirty(true)
{
}

MappedConstantBuffer::~MappedConstantBuffer(void)
{

}

void MappedConstantBuffer::SetValueRaw(ShaderUniformPosition position, const void* data, size_t dataSize)
{
	assert(position.IsValid() && "not a valid layout");
	assert(position.GetSize() == 0 || position.GetSize() >= dataSize && "value doesn't fit");
	assert(position.GetOffset() + dataSize <= mBuffer.size() && "value doesn't fit in buffer");

	memcpy(&mBuffer[position.GetOffset()], data, dataSize);

	mIsDirty = true;
}

void MappedConstantBuffer::UpdateConstantBuffer()
{
	if (mIsDirty)
	{
		mConstantBuffer->Set(&mBuffer.front(), mBuffer.size());
		mIsDirty = false;
	}
}

const ConstantBuffer::Ptr& MappedConstantBuffer::GetConstantBuffer() const
{
	return mConstantBuffer;
}

ShaderUniformPosition MappedConstantBuffer::GetUniform(const std::string& name) const
{
	return mBlockLayout.GetUniform(name);
}

void MappedConstantBuffer::GetUniformNames(std::set<std::string>& output) const
{
	mBlockLayout.GetUniformNames(output);
}


MappedUniformAddress::MappedUniformAddress(const MappedConstantBuffer::Ptr& buffer, ShaderUniformPosition position) :
	mBuffer(buffer),
	mPosition(position)
{

}

bool MappedUniformAddress::MapsToBuffer(const MappedConstantBuffer& buffer) const
{
	return mBuffer.get() == &buffer;
}

MappedConstantBufferRelay::MappedConstantBufferRelay(void)
{

}

MappedConstantBufferRelay::~MappedConstantBufferRelay(void)
{

}

void MappedConstantBufferRelay::AddBuffer(const MappedConstantBuffer::Ptr& value)
{
	set<string> variableNames;
	value->GetUniformNames(variableNames);
	for (auto it = variableNames.begin(); it != variableNames.end(); ++it)
	{
		mUniformMappings[*it].push_back(MappedUniformAddress(value, value->GetUniform(*it)));
	}

	mMappedBuffers.push_back(value);
}

void MappedConstantBufferRelay::RemoveBuffer(const MappedConstantBuffer::Ptr& value)
{
	set<string> variableNames;
	value->GetUniformNames(variableNames);
	for (auto it = variableNames.begin(); it != variableNames.end(); ++it)
	{
		vector<MappedUniformAddress>& addressList = mUniformMappings[*it];

		for (auto address = addressList.begin(); address != addressList.end();)
		{
			if (address->MapsToBuffer(*value))
			{
				address = addressList.erase(address);
			}
			else
			{
				++address;
			}
		}
		mUniformMappings[*it].push_back(MappedUniformAddress(value, value->GetUniform(*it)));
	}

	mMappedBuffers.erase(remove(mMappedBuffers.begin(), mMappedBuffers.end(), value));
}

void MappedConstantBufferRelay::AddAllFrom(const MappedConstantBufferRelay& value)
{
	for (auto mappedBuffer = value.mMappedBuffers.begin(); mappedBuffer != value.mMappedBuffers.end(); ++mappedBuffer)
	{
		AddBuffer(*mappedBuffer);
	}
}

void MappedConstantBufferRelay::RemoveAllFrom(const MappedConstantBufferRelay& value)
{
	for (auto mappedBuffer = value.mMappedBuffers.begin(); mappedBuffer != value.mMappedBuffers.end(); ++mappedBuffer)
	{
		RemoveBuffer(*mappedBuffer);
	}
}


}
