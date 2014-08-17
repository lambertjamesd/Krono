#include "ShaderLoader.h"

#include "ResourceManager.h"

VertexShaderLoader::VertexShaderLoader(void)
{
}


VertexShaderLoader::~VertexShaderLoader(void)
{
}

Auto<Object> VertexShaderLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	return resourceManager.GetGraphics()->CreateVertexShader(StringFromIStream(inputStream));
}


PixelShaderLoader::PixelShaderLoader(void)
{
}


PixelShaderLoader::~PixelShaderLoader(void)
{
}

Auto<Object> PixelShaderLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	return resourceManager.GetGraphics()->CreatePixelShader(StringFromIStream(inputStream));
}
