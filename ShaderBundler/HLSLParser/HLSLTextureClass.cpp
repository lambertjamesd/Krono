#include "HLSLTextureClass.h"
#include <cassert>
#include <sstream>

using namespace std;

HLSLTextureClass::HLSLTextureClass(void)
{
	PopulateFunctions();
}


HLSLTextureClass::~HLSLTextureClass(void)
{
}

void HLSLTextureClass::PopulateFunctions()
{
	mFunctionDefinitions[HLSLType::Texture2D]["Sample"] = CreateSamplerDefinition(2, true);
	mFunctionDefinitions[HLSLType::Texture2D]["Load"] = CreateLoadDefintion(2, false);
	mFunctionDefinitions[HLSLType::Texture2D]["SampleLevel"] = CreateSamplerLevelDefinition(2, true);
}

HLSLTextureClass& HLSLTextureClass::GetInstance()
{
	if (gInstance == NULL)
	{
		gInstance = new HLSLTextureClass();
	}
	return *gInstance;
}

HLSLType HLSLTextureClass::GetMemberType(HLSLType::TextureType textureType, const std::string& name)
{
	HLSLFunctionDefinition* functionResult = mFunctionDefinitions[textureType][name];

	if (functionResult != NULL)
	{
		return HLSLType(*functionResult);
	}

	return HLSLType();
}

HLSLTextureClass* HLSLTextureClass::gInstance = NULL;

HLSLFunctionDefinition* HLSLTextureClass::CreateLoadDefintion(int dimentionality, bool multisampled)
{
	int locationSize = dimentionality;

	if (!multisampled)
	{
		++locationSize;
	}

	ostringstream functionSignature;
	functionSignature << "float4 Load(int" << locationSize << " location, int sampleIndex = 0";
	functionSignature << ", int" << dimentionality << " offset = int" << dimentionality << "(0";

	for (int i = 1; i < dimentionality; ++i)
	{
		functionSignature << ", 0";
	}

	functionSignature << "));";

	istringstream parserInput(functionSignature.str());

	HLSLParser parser(parserInput);

	HLSLFunctionDefinition* result = parser.ParseFunctionSignature().release();
	result->Accept(mTypeResolver);
	return result;
}

HLSLFunctionDefinition* HLSLTextureClass::CreateSamplerDefinition(int dimentionality, bool includeOffset)
{
	ostringstream functionSignature;
	functionSignature << "float4 Sample(SamplerState samplerState, float" << dimentionality << " location";

	if (includeOffset)
	{
		int offsetSize = dimentionality + 1;
		functionSignature << ", int" << offsetSize << " offset = int" << offsetSize << "(0";

		for (int i = 1; i < offsetSize; ++i)
		{
			functionSignature << ", 0";
		}

		functionSignature << ")";
	}

	functionSignature << ");";

	istringstream parserInput(functionSignature.str());

	HLSLParser parser(parserInput);

	HLSLFunctionDefinition* result = parser.ParseFunctionSignature().release();
	result->Accept(mTypeResolver);
	return result;
}

HLSLFunctionDefinition* HLSLTextureClass::CreateSamplerLevelDefinition(int dimentionality, bool includeOffset)
{
	ostringstream functionSignature;
	functionSignature << "float4 SampleLevel(SamplerState samplerState, float" << dimentionality << " location, float lod";

	if (includeOffset)
	{
		int offsetSize = dimentionality + 1;
		functionSignature << ", int" << offsetSize << " offset = int" << offsetSize << "(0";

		for (int i = 1; i < offsetSize; ++i)
		{
			functionSignature << ", 0";
		}

		functionSignature << ")";
	}

	functionSignature << ");";

	istringstream parserInput(functionSignature.str());

	HLSLParser parser(parserInput);

	HLSLFunctionDefinition* result = parser.ParseFunctionSignature().release();
	result->Accept(mTypeResolver);
	return result;
}