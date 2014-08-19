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

class BundleDefinition
{
public:
	
	// This enum must match the enum Graphics::ShaderLanguage in Interface/Graphics.h
	enum ShaderLanguage
	{
		HLSL_5,
		GLSL_4_4,

		LanguageCount,

		InvalidLanguage
	};

	BundleDefinition(const std::string& filename);
	~BundleDefinition(void);

	bool HasLanguage(ShaderLanguage language) const;
	std::vector<ShaderLanguage> GetBoundLanguages() const;
	std::string GetFilename(ShaderLanguage langauge) const;
	ShaderType::Type GetType() const;
	const std::string& GetEntryPoint() const;
private:
	std::map<ShaderLanguage, std::string> mSources;
	ShaderType::Type mType;
	std::string mEntryPoint;

	void ParseDefinition(std::istream& input);

	static const char* gShaderTypes[ShaderType::Count];
	static ShaderType::Type ShaderTypeNameToType(const std::string& name);

	static const char* gShaderLanguageNames[ShaderLanguage::LanguageCount];
	static ShaderLanguage LanguageNameToType(const std::string& name);
};

