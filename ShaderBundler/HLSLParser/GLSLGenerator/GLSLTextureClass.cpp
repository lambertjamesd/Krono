#include "GLSLTextureClass.h"


GLSLTextureClass::GLSLTextureClass(void)
{
	PopulateClass();
}


GLSLTextureClass::~GLSLTextureClass(void)
{
}

GLSLTextureClass& GLSLTextureClass::GetInstance()
{
	if (gInstance == NULL)
	{
		gInstance = new GLSLTextureClass();
	}

	return *gInstance;
}

const std::string& GLSLTextureClass::GetBuiltInFunctions()
{
	return gBuiltInFunctions;
}

GLSLTextureClass* GLSLTextureClass::gInstance = NULL;

std::string GLSLTextureClass::gBuiltInFunctions = 
	"vec4 loadTexture2D(sampler2D sampler, ivec3 location)\n"
	"{\n"
	"\treturn texelFetch(sampler, location.xy, location.z);\n"
	"}";

void GLSLTextureClass::PopulateClass()
{
	mFunctionNameMapping[HLSLType::Texture2D]["Sample"] = "texture2D";
	mFunctionNameMapping[HLSLType::Texture2D]["Load"] = "loadTexture2D";
	mFunctionNameMapping[HLSLType::Texture2D]["SampleLevel"] = "textureLod";
	mFunctionOffsetNameMapping[HLSLType::Texture2D]["Sample"] = "textureOffset2D";
	mFunctionOffsetNameMapping[HLSLType::Texture2D]["Load"] = "loadTextureOffset2D";
}

std::string GLSLTextureClass::GetFunctionMapping(HLSLType::TextureType textureType, const std::string& functionName, size_t parameterCount)
{
	bool useOffset = functionName == "Sample" && parameterCount == 2 || 
		functionName == "SampleLevel" && parameterCount == 3 ||
		functionName == "Load" && parameterCount == 3;

	if (useOffset)
	{
		return mFunctionOffsetNameMapping[textureType][functionName];
	}
	else
	{
		return mFunctionNameMapping[textureType][functionName];
	}
}