#pragma once

#include "HLSLStatementNode.h"
#include "HLSLParser.h"
#include "HLSLTypeVisitor.h"
#include <map>
#include <string>

class HLSLTextureClass
{
public:
	~HLSLTextureClass(void);

	static HLSLTextureClass& GetInstance();

	HLSLType GetMemberType(HLSLType::TextureType textureType, const std::string& name);
private:
	HLSLTextureClass(void);

	void PopulateFunctions();

	static HLSLTextureClass *gInstance;

	HLSLTypeVisitor mTypeResolver;

	HLSLFunctionDefinition* CreateSamplerDefinition(int dimentionality, bool includeOffset);
	HLSLFunctionDefinition* CreateSamplerLevelDefinition(int dimentionality, bool includeOffset);

	std::map<std::string, HLSLFunctionDefinition*> mFunctionDefinitions[HLSLType::TextureTypeCount];
};

