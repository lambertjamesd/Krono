#pragma once

#include "GLSLGenerator.h"

class GLSLEntryPointBuilder
{
public:
	GLSLEntryPointBuilder(GLSLGenerator& generator, ShaderType::Type type, HLSLFunctionDefinition* entryPoint);

	void GenerateEntryPoint();
private:
	static void CreateAttributeNames(HLSLStructDefinition& structure, std::map<std::string, HLSLTypeNode*>& names);
	static void CreateAttributeNames(HLSLFunctionDefinition& structure, std::map<std::string, HLSLTypeNode*>& names);
	static void CreateAttributeNames(HLSLTypeNode& type, const std::string& semantic, std::map<std::string, HLSLTypeNode*>& names);

	void GenerateStructureCopy(HLSLStructDefinition& structure, const std::string& structureName, const std::string& prefix, bool assignIntoStruct);

	void OutputParameterInput(const std::string& inputPrefix, HLSLFunctionDefinition& type);

	void GenerateInOut(bool isInput, const std::string& prefix, const std::map<std::string, HLSLTypeNode*>& names, bool forceLayout = false);

	const char* GetGLSemanticName(const std::string& semanticName);

	GLSLGenerator& mGenerator;
	std::ostream& mOutput;
	ShaderType::Type mShaderType;
	HLSLFunctionDefinition* mEntryPoint;
};