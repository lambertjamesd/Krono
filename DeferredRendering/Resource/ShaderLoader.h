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

	virtual Auto<Object> BuildShader(ResourceManager& resourceManager, const std::string& source) = 0;
private:
	ShaderStage::Type mStage;

	template <typename T>
	T ReadInt(std::istream& input)
	{
		T result;
		input.read((char*)&result, sizeof(T));
		return result;
	}

	static const std::string gHeaderString;
};

class VertexShaderLoader : public ShaderLoader
{
public:
	VertexShaderLoader(void);
	~VertexShaderLoader(void);
protected:
	virtual Auto<Object> BuildShader(ResourceManager& resourceManager, const std::string& source);
};

class PixelShaderLoader : public ShaderLoader
{
public:
	PixelShaderLoader(void);
	~PixelShaderLoader(void);
protected:
	virtual Auto<Object> BuildShader(ResourceManager& resourceManager, const std::string& source);
};

}
