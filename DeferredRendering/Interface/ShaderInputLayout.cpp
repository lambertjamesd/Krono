#include "ShaderInputLayout.h"

namespace krono
{

ShaderUniformPosition::ShaderUniformPosition() :
	mOffset(InvalidOffset),
	mSize(0)
{

}

ShaderUniformPosition::ShaderUniformPosition(size_t offset, size_t size) :
	mOffset(offset),
	mSize(size)
{

}

size_t ShaderUniformPosition::GetOffset() const
{
	return mOffset;
}

size_t ShaderUniformPosition::GetSize() const
{
	return mSize;
}

bool ShaderUniformPosition::IsValid() const
{
	return mOffset != InvalidOffset;
}
	
ShaderUniformBlockLayout::ShaderUniformBlockLayout() :
	mRegister(0),
	mSize(0)
{

}

ShaderUniformBlockLayout::ShaderUniformBlockLayout(size_t registerIndex) :
	mRegister(registerIndex),
	mSize(0)
{

}

void ShaderUniformBlockLayout::AddUniform(const std::string& name, size_t offset, size_t size)
{
	mUniforms[name] = ShaderUniformPosition(offset, size);

	mSize = std::max(mSize, offset + size);
}


void ShaderUniformBlockLayout::GetUniformNames(std::set<std::string>& output) const
{
	for (auto it = mUniforms.begin(); it != mUniforms.end(); ++it)
	{
		output.insert(it->first);
	}
}

ShaderUniformPosition ShaderUniformBlockLayout::GetUniform(const std::string& name) const
{
	auto findResult = mUniforms.find(name);

	if (findResult == mUniforms.end())
	{
		return ShaderUniformPosition();
	}
	else
	{
		return findResult->second;
	}
}


size_t ShaderUniformBlockLayout::GetRegister() const
{
	return mRegister;
}

size_t ShaderUniformBlockLayout::GetSize() const
{
	return mSize;
}

ShaderInputLayout::ShaderInputLayout(void)
{
}


ShaderInputLayout::~ShaderInputLayout(void)
{
}

void ShaderInputLayout::AddUniformBlock(const std::string& name, const ShaderUniformBlockLayout& uniformBlock)
{
	mUniformBlocks[name] = uniformBlock;
}

const ShaderUniformBlockLayout* ShaderInputLayout::GetUniformBlock(const std::string& name) const
{
	auto findResult = mUniformBlocks.find(name);

	if (findResult == mUniformBlocks.end())
	{
		return NULL;
	}
	else
	{
		return &(findResult->second);
	}
}

const ShaderUniformBlockLayout* ShaderInputLayout::GetUniformBlock(size_t registerIndex) const
{
	for (auto it = mUniformBlocks.begin(); it != mUniformBlocks.end(); ++it)
	{
		if (it->second.GetRegister() == registerIndex)
		{
			return &(it->second);
		}
	}

	return NULL;
}

}