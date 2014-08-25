#pragma once

#include "Interface/Shader.h"
#include "Core/Hash.h"
#include "OpenGLConstantBuffer.h"
#include <vector>

namespace krono
{

class OpenGLConstantBufferStorage;

class OpenGLConstantBufferMapping
{
public:
	void AddConstantBuffer(ShaderStage::Type stage, size_t uniformIndex);

	UInt32 GetMappingID() const;
private:
	friend class OpenGLConstantBufferStorage;

	struct Element
	{
		Element(ShaderStage::Type stage, size_t uniformIndex);

		ShaderStage::Type stage;
		size_t uniformIndex;
		
		bool operator==(const Element& other) const;
	};

	bool operator==(const OpenGLConstantBufferMapping& other) const;

	std::vector<Element> mElements;
	HashUInt32 mHash;
};

class OpenGLConstantBufferStorage
{
public:
	OpenGLConstantBufferStorage();

	void SetConstantBuffer(const OpenGLConstantBuffer::Ptr& buffer, size_t index, ShaderStage::Type stage);

	void UseMapping(const OpenGLConstantBufferMapping& mapping);

	void UpdateBindings();
private:
	const OpenGLConstantBuffer* GetConstantBuffer(size_t slot, ShaderStage::Type stage);

	std::vector<OpenGLConstantBuffer::Ptr> mConstantBuffers[ShaderStage::TypeCount];

	OpenGLConstantBufferMapping mCurrentMapping;

	bool mIsDirty;
};

}
