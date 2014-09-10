#pragma once

#include "HLSLNodeVisitor.h"
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <set>

class HLSLTypeStorage
{
public:
	enum TypeSource
	{
		None,
		FunctionType,
		VariableType,
		DefineType
	};

	HLSLTypeStorage();

	void StartScope();
	void EndScope();

	HLSLTypeNode* GetDefinedType(const std::string& name);
	std::shared_ptr<HLSLTypeNode> GetDefinedTypePtr(const std::string& name);
	HLSLType GetVariableType(const std::string& name);
	HLSLType GetFunction(const std::string& name);

	void Define(const HLSLToken& startToken, const std::string& name, const std::shared_ptr<HLSLTypeNode>& value);
	void DefineVariable(const HLSLToken& startToken, const std::string& name, const HLSLType& type);
	void DefineFunction(HLSLFunctionDefinition& value);

	bool IsDefinedInScope(const std::string& name);

	TypeSource GetTypeSource(const std::string& name) const;
private:
	
	void Define(const std::string& name, const std::shared_ptr<HLSLTypeNode>& value);
	
	static std::unique_ptr<HLSLScalarTypeNode> CreateScalarType(HLSLKeyword::Type scalarType);
	static std::shared_ptr<HLSLTypeNode> CreateVectorType(HLSLKeyword::Type scalarType, size_t size);
	static std::shared_ptr<HLSLTypeNode> CreateMatrixType(HLSLKeyword::Type scalarType, size_t rows, size_t columns);

	std::vector<std::map<std::string, TypeSource> > mCurrentScope;
	std::map<std::string, HLSLFunctionDefinition*> mFunctionStorage;
	std::map<std::string, std::stack<HLSLType> > mVariableTypes;
	std::map<std::string, std::stack<std::shared_ptr<HLSLTypeNode> > > mDefinedTypes;
};

class HLSLTypeVisitor : public HLSLNodeVisitor
{
public:
	HLSLTypeVisitor(void);
	~HLSLTypeVisitor(void);
	
	virtual void Visit(HLSLStatementBlock& node);
	virtual void Visit(HLSLTypedefDefinition& node);
	virtual void Visit(HLSLVariableDefinition& node);
	virtual void Visit(HLSLFunctionParameter& node);
	virtual void Visit(HLSLFunctionDefinition& node);
	virtual void Visit(HLSLReturnStatement& node);
	virtual void Visit(HLSLStructureMember& node);
	virtual void Visit(HLSLStructDefinition& node);
	virtual void Visit(HLSLCBufferDefinition& node);
	
	virtual void Visit(HLSLVoidNode& node);
	virtual void Visit(HLSLNamedTypeNode& node);
	virtual void Visit(HLSLScalarTypeNode& node);
	virtual void Visit(HLSLBufferTypeNode& node);
	virtual void Visit(HLSLArrayTypeNode& node);
	virtual void Visit(HLSLVectorTypeNode& node);
	virtual void Visit(HLSLMatrixTypeNode& node);
	virtual void Visit(HLSLTextureTypeNode& node);
	virtual void Visit(HLSLSamplerTypeNode& node);
	virtual void Visit(HLSLStructTypeNode& node);
	virtual void Visit(HLSLFunctionTypeNode& node);
	
	void CheckBooleanExpression(HLSLExpressionNode& node);
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
	
	virtual void Visit(HLSLPrefixIncrNode& node);
	virtual void Visit(HLSLPrefixDecrNode& node);
	
	virtual void Visit(HLSLIndexNode& node);
	virtual void Visit(HLSLPostfixIncr& node);
	virtual void Visit(HLSLPostfixDecr& node);
	virtual void Visit(HLSLStructureNode& node);
	virtual void Visit(HLSLFunctionCallNode& node);
private:
	static bool IsVectorSwizzle(char character, size_t vectorSize);
	static bool IsColorSwizzle(char character, size_t vectorSize);

	static bool IsZeroIndexSwizzle(const char* str, size_t rows, size_t columns);
	static bool IsOneIndexSwizzle(const char* str, size_t rows, size_t columns);

	static HLSLType ParseVectorSwizzle(HLSLType::ScalarType scalar, const std::string& value, size_t vectorSize);
	static HLSLType ParseMatrixSwizzle(HLSLType::ScalarType scalar, const std::string& value, size_t rows, size_t column);

	HLSLTypeStorage mTypeStorage;
	HLSLFunctionDefinition* mCurrentFunction;
};

