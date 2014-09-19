#pragma once

#include "HLSLTypeVisitor.h"

#include <vector>
#include <string>
#include <ostream>

class ShaderUniformDefinition
{
public:
	ShaderUniformDefinition(const std::string& name, size_t offset, size_t size);

	const std::string& GetName() const;
	size_t GetOffset() const;
	size_t GetSize() const;
	
	void WriteDefinition(std::ostream& output);
private:
	std::string mName;
	size_t mOffset;
	size_t mSize;
};

class ShaderUniformBufferDefinition
{
public:
	ShaderUniformBufferDefinition(const std::string& name, size_t registerIndex);

	void AddUniform(const std::string& name, size_t size);

	size_t GetUniformCount() const;
	const ShaderUniformDefinition& GetUniform(size_t index) const;
	
	void WriteDefinition(std::ostream& output);
private:
	size_t RemainingRegisterMemory() const;
	void NextRegister();

	static const size_t RegisterSize = sizeof(float) * 4;
	static const size_t RegisterMask = 0xF;

	std::string mName;
	std::vector<ShaderUniformDefinition> mUniforms;
	size_t mRegister;
	size_t mSize;
};

class ShaderInputDefinition
{
public:
	ShaderInputDefinition();

	ShaderUniformBufferDefinition* AddBufferDefinition(const std::string& name, size_t registerIndex);

	size_t GetBufferCount() const;
	const ShaderUniformBufferDefinition& GetBufferDefinition(size_t index);

	void WriteDefinition(std::ostream& output);
private:
	std::vector<ShaderUniformBufferDefinition> mUniformBuffers;
};

class ShaderInputBuilder : public HLSLTypeVisitor
{
public:
	ShaderInputBuilder(void);
	~ShaderInputBuilder(void);

	static void Process(HLSLNode& file, std::ostream& output);

	virtual void Visit(HLSLStatementBlock& node);
	virtual void Visit(HLSLTypedefDefinition& node);
	virtual void Visit(HLSLVariableDefinition& node);
	virtual void Visit(HLSLFunctionDefinition& node);
	virtual void GenerateStructVariables(HLSLStructDefinition& definition, const std::string& variableName) const;
	virtual void Visit(HLSLStructDefinition& node);
	virtual void Visit(HLSLCBufferDefinition& node);

	static void WriteString(std::ostream& output, const std::string& value);
private:

	ShaderInputDefinition mInputDefinition;
	ShaderUniformBufferDefinition *mCurrentUniformBuffer;
};

