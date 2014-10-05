#pragma once

#include <iostream>
#include "HLSLTokenizer.h"
#include "HLSLNode.h"
#include "HLSLStatementNode.h"
#include "HLSLTypeNode.h"
#include "HLSLFlowControlNode.h"
#include "HLSLBinaryOperatorNode.h"
#include "HLSLUnaryOperator.h"
#include "HLSLPrefixNode.h"
#include "HLSLPostfixNode.h"
#include "Bundle/BundleDefinition.h"
#include <functional>
#include <string>

class HLSLParser
{
public:
	HLSLParser(std::istream& input);
	~HLSLParser(void);

	std::unique_ptr<HLSLNode> ParseFile();
	std::unique_ptr<HLSLFunctionDefinition> ParseFunctionSignature();

	static std::unique_ptr<HLSLNode> ProcessFile(const std::string& filename, const std::map<std::string, std::string>& macros);

	static void Test();
private:
	static void TestString(const std::string& string);
	static const char* gParseTest;

	HLSLTokenizer mTokenizer;
	size_t mCurrentToken;

	size_t GetParseLocation() const;
	void SetParseLocation(size_t value);

	const HLSLToken& Peek(size_t offset);
	const HLSLToken& Require(HLSLTokenType::Type type);
	const HLSLToken& Require(HLSLKeyword::Type type);
	bool Optional(HLSLTokenType::Type type);
	bool Optional(HLSLKeyword::Type type);
	void Advance(size_t count);
	const HLSLToken& Advance();

	bool DryParseType();
	bool DryParseBasicType();
	bool DryParseScalarType();
	bool MatchesType();
	bool MatchesVariableDeclaration();
	bool MatchesFunctionDeclaration();
	bool MatchesControlStatement();

	std::unique_ptr<HLSLNode> ParseFileDeclaration();
	
	std::unique_ptr<HLSLStructDefinition> ParseStruct();
	std::unique_ptr<HLSLStructureMember> ParseStructMember();
	InterpolationMode::Type ParseInterpolationMode();
	std::unique_ptr<HLSLCBufferDefinition> ParseCBuffer();
	HLSLRegisterLocation ParseRegister();
	HLSLRegisterLocation ParsePackOffset();
	void CheckForArrayType(std::unique_ptr<HLSLTypeNode>& type);
	std::unique_ptr<HLSLTypedefDefinition> ParseTypedef();
	std::unique_ptr<HLSLVariableDefinition> ParseVariable();
	VariableStorageClass::Type ParseVariableStorageClass();
	VariableTypeModifer::Type ParseVariableTypeModifier();

	HLSLFunctionParameter::InputModifier ParseInputModifier();
	std::unique_ptr<HLSLFunctionParameter> ParseFunctionParameter();
	std::unique_ptr<HLSLFunctionDefinition> ParseFunction();
	
	std::unique_ptr<HLSLTypeNode> ParseType();
	std::unique_ptr<HLSLTypeNode> ParseBasicType();
	std::unique_ptr<HLSLTypeNode> ParseScalarType();

	std::unique_ptr<HLSLStatementBlock> ParseBlock(bool allowSingleLine);
	std::unique_ptr<HLSLNode> ParseStatement();

	std::unique_ptr<HLSLFlowControlNode> ParseFlowControl();
	std::unique_ptr<HLSLDoNode> ParseDoBlock();
	std::unique_ptr<HLSLIfNode> ParseIfBlock();
	std::unique_ptr<HLSLForNode> ParseForBlock();
	std::unique_ptr<HLSLSwitchNode> ParseSwitchBlock();
	std::unique_ptr<HLSLWhileNode> ParseWhileBlock();

	std::unique_ptr<HLSLSwitchLabelNode> ParseSwitchLabel();
	
	bool MatchesCast();

	std::unique_ptr<HLSLExpressionNode> ParseExpression();
	std::unique_ptr<HLSLExpressionNode> ParseBinaryOperator(OperatorPrecedence::Type minPrecedence);
	std::unique_ptr<HLSLExpressionNode> ParseUnaryOperator();
	std::unique_ptr<HLSLExpressionNode> ParsePrefixExpression();
	std::unique_ptr<HLSLExpressionNode> ParsePostfixExpression();
	std::unique_ptr<HLSLExpressionNode> ParseValueExpression();
};

