#include "GLSLIDRename.h"


GLSLIDRename::GLSLIDRename(void)
{
	PopulateNames();
}


GLSLIDRename::~GLSLIDRename(void)
{
}

const std::string& GLSLIDRename::RenameID(const std::string& input) const
{
	auto result = mRenameMap.find(input);

	if (result != mRenameMap.end())
	{
		return result->second;
	}
	else
	{
		return input;
	}
}

void GLSLIDRename::AddRename(const std::string& oldName, const std::string& newName)
{
	mRenameMap[oldName] = newName;
}

void GLSLIDRename::PopulateNames()
{
	mRenameMap["frac"] = "fract";
	mRenameMap["lerp"] = "mix";
	mRenameMap["main"] = "main__rename__";
	mRenameMap["input"] = "input__rename__";
	
	mRenameMap["ddx"] = "dFdx";
	mRenameMap["ddy"] = "dFdy";
}