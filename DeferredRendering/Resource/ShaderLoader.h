#pragma once

#include "ResourceLoader.h"

namespace krono
{

class ShaderLoader : public ResourceLoader
{
public:
	~ShaderLoader();
	
	virtual Auto<Object> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
protected:
	ShaderLoader(ShaderStage::Type stage);

	virtual Shader::Ptr BuildShader(ResourceManager& resourceManager, const std::string& source) = 0;
private:
	bool LoadShaderOfType(std::istream& inputStream, Graphics::ShaderLanguage language, std::string& output);

	ShaderStage::Type mStage;

	template <typename T>
	static T ReadInt(std::istream& input)
	{
		T result;
		input.read((char*)&result, sizeof(T));
		return result;
	}

	static bool ParseInputLayout(std::istream& inputStream, ShaderInputLayout& output);
	static bool ParseUniformBlockLayout(std::istream& inputStream, ShaderUniformBlockLayout& output);
	static bool ParseString(std::istream& inputStream, std::string& output);

	static const std::string gHeaderString;
	static const Graphics::ShaderLanguage ShaderInputLayoutIdentifier = (Graphics::ShaderLanguage)0xFF;
};

class VertexShaderLoader : public ShaderLoader
{
public:
	VertexShaderLoader(void);
	~VertexShaderLoader(void);
protected:
	virtual Shader::Ptr BuildShader(ResourceManager& resourceManager, const std::string& source);
};

class PixelShaderLoader : public ShaderLoader
{
public:
	PixelShaderLoader(void);
	~PixelShaderLoader(void);
protected:
	virtual Shader::Ptr BuildShader(ResourceManager& resourceManager, const std::string& source);
};

}
