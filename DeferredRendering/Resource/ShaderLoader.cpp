#include "ShaderLoader.h"

#include "ResourceManager.h"
#include "FormatException.h"
#include "NotFoundException.h"


const std::string ShaderLoader::gHeaderString = "SHDR";

ShaderLoader::ShaderLoader(ShaderStage::Type stage) :
	mStage(stage)
{

}

ShaderLoader::~ShaderLoader()
{

}

Auto<Object> ShaderLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	std::string headerRead;
	headerRead.resize(gHeaderString.length());
	inputStream.read(&headerRead.front(), gHeaderString.length());

	if (headerRead != gHeaderString)
	{
		throw FormatException("Invalid shader file, use the ShaderBundler utility to build shaders");
	}

	ShaderStage::Type stage = static_cast<ShaderStage::Type>(ReadInt<short>(inputStream));
	short languageCount = ReadInt<short>(inputStream);

	if (stage != mStage)
	{
		throw FormatException("Expected shader stage did not match type in file");
	}

	for (short i = 0; i < languageCount; ++i)
	{
		short shaderLanguage;
		long fileOffset;
		long fileLength;

		inputStream.read((char*)&shaderLanguage, sizeof(short));
		inputStream.read((char*)&fileOffset, sizeof(long));
		inputStream.read((char*)&fileLength, sizeof(long));

		if ((Graphics::ShaderLanguage)shaderLanguage == resourceManager.GetGraphics()->ExpectedShaderLanguage())
		{
			inputStream.seekg(fileOffset, std::ios_base::beg);
			std::string buffer;
			buffer.resize(fileLength);
			inputStream.read(&buffer.front(), fileLength);

			return BuildShader(resourceManager, buffer);
		}
	}

	throw NotFoundException("Expected shader language not found in file");
}

VertexShaderLoader::VertexShaderLoader(void) :
	ShaderLoader(ShaderStage::VertexShader)
{
}


VertexShaderLoader::~VertexShaderLoader(void)
{
}

Auto<Object> VertexShaderLoader::BuildShader(ResourceManager& resourceManager, const std::string& source)
{
	return resourceManager.GetGraphics()->CreateVertexShader(source);
}


PixelShaderLoader::PixelShaderLoader(void) :
	ShaderLoader(ShaderStage::PixelShader)
{
}


PixelShaderLoader::~PixelShaderLoader(void)
{
}

Auto<Object> PixelShaderLoader::BuildShader(ResourceManager& resourceManager, const std::string& source)
{
	return resourceManager.GetGraphics()->CreatePixelShader(source);
}
