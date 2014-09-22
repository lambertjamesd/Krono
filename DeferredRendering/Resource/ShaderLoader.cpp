#include "ShaderLoader.h"

#include "ResourceManager.h"
#include "FormatException.h"
#include "NotFoundException.h"

#include <sstream>

using namespace std;

namespace krono
{

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

	if (stage != mStage)
	{
		throw FormatException("Expected shader stage did not match type in file");
	}

	streamoff streamPosition = inputStream.tellg();

	std::string buffer;
	if (!LoadShaderOfType(inputStream, resourceManager.GetGraphics()->ExpectedShaderLanguage(), buffer))
	{
		throw NotFoundException("Expected shader language not found in file");
	}

	Shader::Ptr result = BuildShader(resourceManager, buffer);

	inputStream.seekg(streamPosition, std::ios_base::beg);
	
	std::string layoutBuffer;
	if (LoadShaderOfType(inputStream, ShaderInputLayoutIdentifier, layoutBuffer))
	{
		istringstream layoutStream(layoutBuffer);
		ShaderInputLayout inputLayout;
		if (ParseInputLayout(layoutStream, inputLayout))
		{
			result->SetInputLayout(inputLayout);
		}
	}

	return result;
}

bool ShaderLoader::LoadShaderOfType(std::istream& inputStream, Graphics::ShaderLanguage language, std::string& output)
{
	short languageCount = ReadInt<short>(inputStream);

	for (short i = 0; i < languageCount; ++i)
	{
		short shaderLanguage;
		long fileOffset;
		long fileLength;

		inputStream.read((char*)&shaderLanguage, sizeof(short));
		inputStream.read((char*)&fileOffset, sizeof(long));
		inputStream.read((char*)&fileLength, sizeof(long));

		if ((Graphics::ShaderLanguage)shaderLanguage == language)
		{
			inputStream.seekg(fileOffset, std::ios_base::beg);
			output.resize(fileLength);
			inputStream.read(&output.front(), fileLength);
			return true;
		}
	}

	return false;
}


bool ShaderLoader::ParseInputLayout(std::istream& inputStream, ShaderInputLayout& output)
{
	short uniformBufferCount = ReadInt<short>(inputStream);

	for (short i = 0; i < uniformBufferCount; ++i)
	{
		std::string name;

		if (!ParseString(inputStream, name))
		{
			return false;
		}
			
		ShaderUniformBlockLayout uniformBlock;

		if (!ParseUniformBlockLayout(inputStream, uniformBlock))
		{
			return false;
		}

		output.AddUniformBlock(name, uniformBlock);
	}

	return true;
}

bool ShaderLoader::ParseUniformBlockLayout(std::istream& inputStream, ShaderUniformBlockLayout& output)
{
	size_t sizeRegister = ReadInt<size_t>(inputStream);
	size_t uniformCount = sizeRegister & 0xFFFF;
	size_t registerIndex = (sizeRegister >> 16) & 0xFFFF;

	output = ShaderUniformBlockLayout(registerIndex);

	for (size_t i = 0; i < uniformCount; ++i)
	{
		std::string name;

		if (!ParseString(inputStream, name))
		{
			return false;
		}
		
		size_t sizeOffset = ReadInt<size_t>(inputStream);
		output.AddUniform(name, (sizeOffset >> 16) & 0xFFFF, sizeOffset & 0xFFFF);
	}

	return true;
}

bool ShaderLoader::ParseString(std::istream& inputStream, std::string& output)
{
	short length = ReadInt<short>(inputStream);
	short lengthDoubleCheck = ReadInt<short>(inputStream);

	if (length != lengthDoubleCheck)
	{
		return false;
	}

	output.resize(length);
	inputStream.read(&output.front(), length);

	return true;
}

VertexShaderLoader::VertexShaderLoader(void) :
	ShaderLoader(ShaderStage::VertexShader)
{
}


VertexShaderLoader::~VertexShaderLoader(void)
{
}

Shader::Ptr VertexShaderLoader::BuildShader(ResourceManager& resourceManager, const std::string& source)
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

Shader::Ptr PixelShaderLoader::BuildShader(ResourceManager& resourceManager, const std::string& source)
{
	return resourceManager.GetGraphics()->CreatePixelShader(source);
}

}
