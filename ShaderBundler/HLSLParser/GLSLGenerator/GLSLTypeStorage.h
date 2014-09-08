#pragma once

#include "HLSLParser/HLSLTypeNode.h"
#include "HLSLParser/HLSLExpressionNode.h"

#include <map>
#include <string>
#include <memory>
#include <vector>

class GLSLTypeStorage
{
public:
	GLSLTypeStorage(void);
	~GLSLTypeStorage(void);

	void BeginScope();
	void EndScope();
	void Define(const std::string& name, const std::shared_ptr<HLSLNode>& value);

	void DefineFunction(HLSLFunctionDefinition& value);

	HLSLNode* GetValue(const std::string& name);
private:
	std::map<std::string, std::vector<std::shared_ptr<HLSLNode> > > mDefinedValues;
	std::vector<std::vector<std::string> > mScopeDefines;
	
	static std::unique_ptr<HLSLScalarTypeNode> CreateScalarType(HLSLKeyword::Type scalarType);
	static std::shared_ptr<HLSLNode> CreateVectorType(HLSLKeyword::Type scalarType, size_t size);
	static std::shared_ptr<HLSLNode> CreateMatrixType(HLSLKeyword::Type scalarType, size_t rows, size_t columns);
};

