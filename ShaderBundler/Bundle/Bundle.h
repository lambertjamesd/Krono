#pragma once

#include "BundleDefinition.h"
#include <map>
#include <iostream>

class Bundle
{
public:
	Bundle(const BundleDefinition& bundleDef);
	~Bundle(void);

	void AddShader(ShaderLanguage::Type language, const std::vector<char>& data);

	void Write(std::ostream& output) const;
private:
	ShaderType::Type mType;
	std::map<ShaderLanguage::Type, std::vector<char> > mShaders;

	static const std::string gFileHeader;

	size_t GetShaderOffset(ShaderLanguage::Type language) const;

	template <typename T>
	static void WriteInt(std::ostream& output, T value)
	{
		output.write((char*)&value, sizeof(T));
	}
};

