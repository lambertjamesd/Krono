#include "ShaderInputBuilder.h"
#include "HLSLParserException.h"
#include <cassert>
#include <algorithm>

ShaderUniformDefinition::ShaderUniformDefinition(const std::string& name, size_t offset, size_t size) :
	mName(name),
	mOffset(offset),
	mSize(size)
{

}

const std::string& ShaderUniformDefinition::GetName() const
{
	return mName;
}

size_t ShaderUniformDefinition::GetOffset() const
{
	return mOffset;
}

size_t ShaderUniformDefinition::GetSize() const
{
	return mSize;
}

void ShaderUniformDefinition::WriteDefinition(std::ostream& output)
{
	ShaderInputBuilder::WriteString(output, mName);
	size_t offsetCount = (mSize & 0xFFFF) | (mOffset & 0xFFFF) << 16;
	output.write((char*)&offsetCount, sizeof(size_t));
}

ShaderUniformBufferDefinition::ShaderUniformBufferDefinition(const std::string& name, size_t registerIndex) :
	mName(name),
	mRegister(registerIndex),
	mSize(0)
{

}

void ShaderUniformBufferDefinition::AddUniform(const std::string& name, size_t offset, size_t size)
{
	mUniforms.push_back(ShaderUniformDefinition(name, offset, size));
	mSize = std::max(mSize, offset + size);
}

void ShaderUniformBufferDefinition::AddUniform(const std::string& name, size_t size)
{
	if (RemainingRegisterMemory() < size)
	{
		NextRegister();
	}

	mUniforms.push_back(ShaderUniformDefinition(name, mSize, size));
	mSize += size;
}

size_t ShaderUniformBufferDefinition::GetUniformCount() const
{
	return mSize;
}

const ShaderUniformDefinition& ShaderUniformBufferDefinition::GetUniform(size_t index) const
{
	return mUniforms[index];
}

void ShaderUniformBufferDefinition::WriteDefinition(std::ostream& output)
{
	ShaderInputBuilder::WriteString(output, mName);
	size_t uniformCount = (mUniforms.size() & 0xFFFF) | (mRegister & 0xFFFF) << 16;
	output.write((char*)&uniformCount, sizeof(size_t));

	for (size_t i = 0; i < mUniforms.size(); ++i)
	{
		mUniforms[i].WriteDefinition(output);
	}
}

size_t ShaderUniformBufferDefinition::RemainingRegisterMemory() const
{
	return RegisterSize - mSize & RegisterMask;
}

void ShaderUniformBufferDefinition::NextRegister()
{
	mSize = (mSize + RegisterSize - 1) & ~RegisterMask;
}

ShaderInputDefinition::ShaderInputDefinition()
{

}

ShaderUniformBufferDefinition* ShaderInputDefinition::AddBufferDefinition(const std::string& name, size_t registerIndex)
{
	mUniformBuffers.push_back(ShaderUniformBufferDefinition(name, registerIndex));
	return &mUniformBuffers.back();
}

size_t ShaderInputDefinition::GetBufferCount() const
{
	return mUniformBuffers.size();
}

const ShaderUniformBufferDefinition& ShaderInputDefinition::GetBufferDefinition(size_t index)
{
	return mUniformBuffers[index];
}

void ShaderInputDefinition::WriteDefinition(std::ostream& output)
{
	short bufferCount = mUniformBuffers.size();
	output.write((char*)&bufferCount, sizeof(short));

	for (short i = 0; i < bufferCount; ++i)
	{
		mUniformBuffers[i].WriteDefinition(output);
	}
}

ShaderInputBuilder::ShaderInputBuilder(void) :
	mCurrentUniformBuffer(NULL)
{
}


ShaderInputBuilder::~ShaderInputBuilder(void)
{
}

void ShaderInputBuilder::Process(HLSLNode& file, std::ostream& output)
{
	ShaderInputBuilder inputBuilder;
	file.Accept(inputBuilder);
	inputBuilder.mInputDefinition.WriteDefinition(output);
}

void ShaderInputBuilder::Visit(HLSLStatementBlock& node)
{
	// do nothing
}

void ShaderInputBuilder::Visit(HLSLTypedefDefinition& node)
{
	// do nothing
}

void ShaderInputBuilder::GenerateStructVariables(HLSLStructDefinition& definition, const std::string& variableName) const
{
	for (size_t i = 0; i < definition.GetMemberCount(); ++i)
	{
		HLSLStructureMember& structureDef = definition.GetMember(i);
		HLSLType type = structureDef.GetType().GetType();
		std::string name = variableName + "." + structureDef.GetName();

		if (type.IsNumerical())
		{
			mCurrentUniformBuffer->AddUniform(name, type.GetSize());
		}
		else if (type.GetType() == HLSLType::Struct)
		{
			GenerateStructVariables(type.GetStructure(), name);
		}
		else
		{
			throw HLSLParserException(structureDef.GetToken(), "Invalid type in cbuffer");
		}
	}
}

void ShaderInputBuilder::Visit(HLSLVariableDefinition& node)
{
	if (mCurrentUniformBuffer != NULL)
	{
		HLSLType type = node.GetType().GetType();

		if (type.IsNumerical())
		{
			HLSLRegisterLocation location = node.GetRegisterLocation();

			if (location.HasValue())
			{
				mCurrentUniformBuffer->AddUniform(node.GetName(), (location.GetRegisterNumber() * 4 + location.GetIndex()) * sizeof(float), type.GetSize());
			}
			else
			{
				mCurrentUniformBuffer->AddUniform(node.GetName(), type.GetSize());
			}
		}
		else if (type.GetType() == HLSLType::Struct)
		{
			GenerateStructVariables(type.GetStructure(), node.GetName());
		}
		else
		{
			throw HLSLParserException(node.GetToken(), "Invalid type in cbuffer");
		}
	}
}

void ShaderInputBuilder::Visit(HLSLFunctionDefinition& node)
{
	// do nothing
}

void ShaderInputBuilder::Visit(HLSLStructDefinition& node)
{
	// do nothing
}

void ShaderInputBuilder::Visit(HLSLCBufferDefinition& node)
{
	mCurrentUniformBuffer = mInputDefinition.AddBufferDefinition(node.GetName(), node.GetRegisterLocation().GetRegisterNumber());

	for (size_t i = 0; i < node.GetMemberCount(); ++i)
	{
		node.GetMember(i).Accept(*this);
	}

	mCurrentUniformBuffer = NULL;
}

void ShaderInputBuilder::WriteString(std::ostream& output, const std::string& value)
{
	unsigned short length = value.length();
	// write the value twice to be able 
	// to detect a corrupt or invalid file
	output.write((char*)&length, sizeof(unsigned short));
	output.write((char*)&length, sizeof(unsigned short));
	output.write(&value.front(), value.length());
}