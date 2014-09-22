#pragma once

#include <map>
#include <string>
#include <set>

#include "Core/Types.h"

namespace krono
{

class ShaderUniformPosition
{
public:
	ShaderUniformPosition();
	ShaderUniformPosition(size_t offset, size_t size);

	size_t GetOffset() const;
	size_t GetSize() const;

	bool IsValid() const;
private:
	size_t mOffset;
	size_t mSize;

	static const size_t InvalidOffset = ~0;
};

class ShaderUniformBlockLayout
{
public:
	ShaderUniformBlockLayout();
	ShaderUniformBlockLayout(size_t registerIndex);

	void AddUniform(const std::string& name, size_t offset, size_t size);

	void GetUniformNames(std::set<std::string>& output) const;
	ShaderUniformPosition GetUniform(const std::string& name) const;

	size_t GetRegister() const;
	size_t GetSize() const;
private:
	std::map<std::string, ShaderUniformPosition> mUniforms;
	size_t mRegister;
	size_t mSize;
};

class ShaderInputLayout
{
public:
	ShaderInputLayout(void);
	~ShaderInputLayout(void);

	void AddUniformBlock(const std::string& name, const ShaderUniformBlockLayout& uniformBlock);
	const ShaderUniformBlockLayout* GetUniformBlock(const std::string& name) const;
	const ShaderUniformBlockLayout* GetUniformBlock(size_t registerIndex) const;
private:
	std::map<std::string, ShaderUniformBlockLayout> mUniformBlocks;
};

}