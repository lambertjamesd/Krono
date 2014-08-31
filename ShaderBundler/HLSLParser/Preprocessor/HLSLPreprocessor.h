#pragma once

#include <istream>
#include <sstream>
#include <map>
#include <string>

#include "HLSLPreprocessorMacro.h"
#include "HLSLPreprocessorTokenizer.h"
#include "HLSLPreprocessorExpressionNode.h"
#include "HLSLPreprocessorNode.h"
#include "HLSLPreprocessorDirectiveNode.h"

class HLSLPreprocessor
{
public:
	HLSLPreprocessor(std::istream& stream, HLSLPreprocessorMacroStorage& definitions);
	~HLSLPreprocessor(void);
private:
	const HLSLPreprocessorToken& Peek(size_t offset);
	const HLSLPreprocessorToken& Require(HLSLPreprocessorToken::Type type);
	void Skip(size_t count);

	bool Optional(HLSLPreprocessorToken::Type type);

	std::unique_ptr<HLSLPreprooessorNodeList> ParseFile();

	std::unique_ptr<HLSLPreprocessorDirectiveNode> ParseDirective();
	std::unique_ptr<HLSLPreprocessorDefineNode> ParseDefine();
	std::unique_ptr<HLSLPreprocessorElifNode> ParseElif();
	std::unique_ptr<HLSLPreprocessorElseNode> ParseElse();
	std::unique_ptr<HLSLPreprocessorEndIfNode> ParseEndIf();
	std::unique_ptr<HLSLPreprocessorErrorNode> ParseError();
	std::unique_ptr<HLSLPreprocessorIfNode> ParseIf();
	std::unique_ptr<HLSLPreprocessorIfDefNode> ParseIfDef();
	std::unique_ptr<HLSLPreprocessorIfNDefNode> ParseIfNDef();
	std::unique_ptr<HLSLPreprocessorIncludeNode> ParseInclude();
	std::unique_ptr<HLSLPreprocessorLineNode> ParseLineDef();
	std::unique_ptr<HLSLPreprocessorPragmaNode> ParsePragma();
	std::unique_ptr<HLSLPreprocessorUnDefNode> ParseUnDef();
	
	std::unique_ptr<HLSLPreprocessorNode> ParseValue();
	std::unique_ptr<HLSLPreprocessorNode> ParseIdentifier();
	std::unique_ptr<HLSLPreprocessorNode> ParseFunctionCall();

	std::unique_ptr<HLSLPreprocessorExpressionNode> ParseExpression();
	
	HLSLPreprocessorTokenizer mTokens;
	HLSLPreprocessorMacroStorage &mDefinitions;
	size_t mCurrentToken;
	std::ostringstream mOutput;
};

