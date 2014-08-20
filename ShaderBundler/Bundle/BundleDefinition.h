#pragma once

#include <string>
#include <map>
#include <fstream>
#include <vector>

namespace ShaderType
{
	enum Type
	{
		VertexShader,
		HullShader,
		DomainShader,
		GeometryShader,
		PixelShader,
		ComputeShader,

		Count,

		Invalid,
	};
}

namespace ShaderLanguage
{
	enum Type
	{
		HLSL_5,
		GLSL_4_4,

		Count,

		Invalid
	};
}

class BundleDefinition
{
public:
	
	// This enum must match the enum Graphics::ShaderLanguage in Interface/Graphics.h

	BundleDefinition(const std::string& filename);
	~BundleDefinition(void);

	bool HasLanguage(ShaderLanguage::Type language) const;
	std::vector<ShaderLanguage::Type> GetBoundLanguages() const;
	std::string GetFilename(ShaderLanguage::Type langauge) const;
	ShaderType::Type GetType() const;
	const std::string& GetEntryPoint() const;
private:
	std::map<ShaderLanguage::Type, std::string> mSources;
	ShaderType::Type mType;
	std::string mEntryPoint;
	std::string mBaseFilename;

	void ParseDefinition(std::istream& input);

	static const char* gShaderTypes[ShaderType::Count];
	static ShaderType::Type ShaderTypeNameToType(const std::string& name);

	static const char* gShaderLanguageNames[ShaderLanguage::Count];
	static ShaderLanguage::Type LanguageNameToType(const std::string& name);
};

