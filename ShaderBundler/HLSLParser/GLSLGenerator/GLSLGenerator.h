#pragma once

#include "HLSLParser/HLSLNodeVisitor.h"
#include "Bundle/BundleDefinition.h"
#include <iostream>
#include <map>
#include <set>
#include "GLSLIDRename.h"

class GLSLPostIndexGenerator : public HLSLNodeVisitor
{
public:
	GLSLPostIndexGenerator(std::ostream& output);
	
	virtual void Visit(HLSLNode& node);
	virtual void Visit(HLSLNamedTypeNode& node);
	virtual void Visit(HLSLArrayTypeNode& node);
private:
	std::ostream& mOutput;
};

class GLSLGenerator : public HLSLNodeVisitor
{
public:
	GLSLGenerator(std::ostream& output, ShaderType::Type type, const std::string& entryPoint);
	~GLSLGenerator(void);

	std::ostream& GetOutput();
	
	static void ProcessFile(HLSLNode& file, ShaderType::Type type, const std::string& entryPoint, std::ostream& output);

	virtual void Visit(HLSLNodeList& node);
	virtual void Visit(HLSLStatementBlock& node);
	virtual void Visit(HLSLTypedefDefinition& node);
	virtual void Visit(HLSLVariableDefinition& node);
	virtual void Visit(HLSLFunctionParameter& node);
	virtual void Visit(HLSLFunctionDefinition& node);
	virtual void Visit(HLSLReturnStatement& node);
	virtual void Visit(HLSLStructureMember& node);
	virtual void Visit(HLSLStructDefinition& node);
	virtual void Visit(HLSLCBufferDefinition& node);

	void OutputVariableDeclaration(const std::string& variableName, HLSLTypeNode& type);
	
	virtual void Visit(HLSLVoidNode& node);
	virtual void Visit(HLSLNamedTypeNode& node);
	virtual void Visit(HLSLScalarTypeNode& node);
	virtual void Visit(HLSLBufferTypeNode& node);
	virtual void Visit(HLSLArrayTypeNode& node);
	std::string GetScalarPrefix(HLSLTypeNode& node);
	virtual void Visit(HLSLVectorTypeNode& node);
	virtual void Visit(HLSLMatrixTypeNode& node);
	virtual void Visit(HLSLTextureTypeNode& node);
	virtual void Visit(HLSLSamplerTypeNode& node);
	virtual void Visit(HLSLStructTypeNode& node);

	virtual void Visit(HLSLDoNode& node);
	virtual void Visit(HLSLIfNode& node);
	virtual void Visit(HLSLForNode& node);
	virtual void Visit(HLSLSwitchNode& node);
	virtual void Visit(HLSLSwitchLabelNode& node);
	virtual void Visit(HLSLDefaultNode& node);
	virtual void Visit(HLSLWhileNode& node);
	virtual void Visit(HLSLBreakNode& node);
	virtual void Visit(HLSLContinueNode& node);
	virtual void Visit(HLSLDiscardNode& node);
	
	virtual void Visit(HLSLIdentifierNode& node);
	virtual void Visit(HLSLNumberConstantNode& node);
	virtual void Visit(HLSLBooleanConstantNode& node);
	virtual void Visit(HLSLTypeExpressionNode& node);
	virtual void Visit(HLSLParenthesisNode& node);
	
	virtual void Visit(HLSLBinaryOperatorNode& node);
	
	virtual void Visit(HLSLUnaryOperator& node);
	virtual void Visit(HLSLCastNode& node);
	
	virtual void Visit(HLSLPrefixNode& node);

	virtual void Visit(HLSLPostfixNode& node);
	virtual void Visit(HLSLIndexNode& node);
	virtual void Visit(HLSLStructureNode& node);
	virtual void Visit(HLSLFunctionCallNode& node);

	const GLSLIDRename& GetIDRename() const;
private:

	void OutputIndents();
	void IncreaseIndent();
	void DecreaseIndent();

	std::ostream& mOutput;
	size_t mIndentLevel;

	static std::string gIndentString;

	static const char* gUniformBufferSuffix[ShaderType::Count];

	bool mNoSemiColon;
	bool mIsInFunction;
	bool mIsInsideCBuffer;
	
	ShaderType::Type mShaderType;
	std::string mEntryPointName;
	HLSLFunctionDefinition* mEntryPoint;

	GLSLIDRename mIDRename;
};

