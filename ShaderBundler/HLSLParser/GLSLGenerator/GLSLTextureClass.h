#pragma once

#include "HLSLParser/HLSLType.h"
#include <map>
#include <string>

class GLSLTextureClass
{
public:
	~GLSLTextureClass(void);

	std::string GetFunctionMapping(HLSLType::TextureType textureType, const std::string& functionName, size_t parameterCount);

	static GLSLTextureClass& GetInstance();

	static const std::string& GetBuiltInFunctions();
private:
	GLSLTextureClass(void);
	void PopulateClass();

	static GLSLTextureClass* gInstance;
	static std::string gBuiltInFunctions;

	std::map<std::string, std::string> mFunctionNameMapping[HLSLType::TextureTypeCount];
	std::map<std::string, std::string> mFunctionOffsetNameMapping[HLSLType::TextureTypeCount];
};

