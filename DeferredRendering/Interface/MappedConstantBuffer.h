#pragma once

#include "ConstantBuffer.h"
#include "ShaderInputLayout.h"

#include <vector>
#include <set>

namespace krono
{

class MappedConstantBuffer
{
public:
	typedef Auto<MappedConstantBuffer> Ptr;

	MappedConstantBuffer(const ConstantBuffer::Ptr& constantBuffer, const ShaderUniformBlockLayout& blockLayout);
	MappedConstantBuffer(const ConstantBuffer::Ptr& constantBuffer, const ShaderUniformBlockLayout& blockLayout, size_t extraBytes);
	~MappedConstantBuffer(void);

	void SetValueRaw(ShaderUniformPosition position, const void* data, size_t dataSize);

	template<typename T>
	void SetValue(ShaderUniformPosition position, const T& value)
	{
		SetValue(position, &value, sizeof(T));
	}

	template<typename T>
	void SetValue(ShaderUniformPosition position, const T* elements, size_t count)
	{
		SetValueRaw(position, elements, sizeof(T) * count);
	}

	void UpdateConstantBuffer();

	const ConstantBuffer::Ptr& GetConstantBuffer() const;
	ShaderUniformPosition GetUniform(const std::string& name) const;
	void GetUniformNames(std::set<std::string>& output) const;
private:
	ConstantBuffer::Ptr mConstantBuffer;
	ShaderUniformBlockLayout mBlockLayout;
	std::vector<char> mBuffer;
	bool mIsDirty;
};

class MappedUniformAddress
{
public:
	MappedUniformAddress(const MappedConstantBuffer::Ptr& buffer, ShaderUniformPosition position);

	template<typename T>
	void SetValue(const T& value)
	{
		mBuffer->SetValue<T>(mPosition, value);
	}
	
	template<typename T>
	void SetValue(const T* elements, size_t elementCount)
	{
		mBuffer->SetValue<T>(mPosition, elements, elementCount);
	}

	bool MapsToBuffer(const MappedConstantBuffer& buffer) const;
private:
	MappedConstantBuffer::Ptr mBuffer;
	ShaderUniformPosition mPosition;
};

class MappedConstantBufferRelay
{
public:
	MappedConstantBufferRelay(void);
	~MappedConstantBufferRelay(void);

	void AddBuffer(const MappedConstantBuffer::Ptr& value);
	void RemoveBuffer(const MappedConstantBuffer::Ptr& value);

	void AddAllFrom(const MappedConstantBufferRelay& value);
	void RemoveAllFrom(const MappedConstantBufferRelay& value);
	
	template<typename T>
	void SetValue(const std::string& name, const T& value)
	{
		SetValue<T>(name, &value, 1);
	}
	
	template<typename T>
	void SetValue(const std::string& name, const T* elements, size_t elementCount)
	{
		auto findResult = mUniformMappings.find(name);

		if (findResult != mUniformMappings.end())
		{
			std::vector<MappedUniformAddress>& addresses = findResult->second;

			for (auto it = addresses.begin(); it != addresses.end(); ++it)
			{
				it->SetValue<T>(elements, elementCount);
			}
		}
	}
private:
	std::map<std::string, std::vector<MappedUniformAddress>> mUniformMappings;
	std::vector<MappedConstantBuffer::Ptr> mMappedBuffers;
};

}