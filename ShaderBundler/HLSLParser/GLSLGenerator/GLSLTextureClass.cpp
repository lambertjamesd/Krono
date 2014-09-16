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

GLSLTextureClass* GLSLTextureClass::gInstance = NULL;

void GLSLTextureClass::PopulateClass()
{
	mFunctionNameMapping[HLSLType::Texture2D]["Sample"] = "texture2D";
	mFunctionOffsetNameMapping[HLSLType::Texture2D]["Sample"] = "textureOffset2D";
}

std::string GLSLTextureClass::GetFunctionMapping(HLSLType::TextureType textureType, const std::string& functionName, bool useOffset)
{
	if (useOffset)
	{
		return mFunctionOffsetNameMapping[textureType][functionName];
	}
	else
	{
		return mFunctionNameMapping[textureType][functionName];
	}
}